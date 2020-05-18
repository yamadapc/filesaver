//
// Created by Pedro Tacla Yamada on 18/05/20.
//

#include <node.h>
#include <node_api.h>
#include <node_object_wrap.h>
#include <vector>

#include <lfilesaver/FileSaver.h>

namespace filesaver::node_addon
{

using filesaver::FileSaver;
using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::ObjectTemplate;
using v8::String;
using v8::Value;

static FileSaver filesaver;

class NodeFileSaver : public node::ObjectWrap
{
public:
    static void initialize (Local<Object> exports)
    {
        auto* isolate = exports->GetIsolate ();
        auto context = isolate->GetCurrentContext ();

        auto addonDataTpl = ObjectTemplate::New (isolate);
        addonDataTpl->SetInternalFieldCount (1);

        auto addonData = addonDataTpl->NewInstance (context).ToLocalChecked ();
        auto name = String::NewFromUtf8 (isolate, "FileSaver", NewStringType::kNormal).ToLocalChecked ();
        auto tpl = FunctionTemplate::New (isolate, New, addonData);
        tpl->SetClassName (name);

        std::vector<std::pair<std::string, v8::FunctionCallback>> methods{
            {"start", Start},
            {"stop", Stop},
            {"scan", Scan},

            {"getCurrentSizeAt", GetCurrentSizeAt},
            {"getTargets", GetTargets},
            {"getTotalFiles", GetTotalFiles},
            {"getTotalKnownFiles", GetTotalKnownFiles},
            {"getFilesPerSecond", GetFilesPerSecond},
            {"getNumWorkers", GetNumWorkers},
            {"getElapsed", GetElapsed},
            {"getStorageQueueSize", GetStorageQueueSize},
            {"getInMemoryEntryCount", GetInMemoryEntryCount},

            {"isPathFinished", IsPathFinished},
            {"areAllTargetsFinished", AreAllTargetsFinished},

            {"setupDefaultStorage", SetupDefaultStorage},
            {"setNumWorkers", SetNumWorkers},
        };

        tpl->InstanceTemplate ()->SetInternalFieldCount (static_cast<int> (methods.size ()));
        for (auto& pair : methods)
        {
            NODE_SET_PROTOTYPE_METHOD (tpl, pair.first.c_str (), pair.second);
        }

        auto constructor = tpl->GetFunction (context).ToLocalChecked ();
        addonData->SetInternalField (0, constructor);
        exports->Set (context, name, constructor).FromJust ();
    }

private:
    NodeFileSaver () = default;
    ~NodeFileSaver () override = default;

    static void New (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        Isolate* isolate = args.GetIsolate ();
        Local<Context> context = isolate->GetCurrentContext ();

        if (args.IsConstructCall ())
        {
            auto* obj = new NodeFileSaver ();
            obj->Wrap (args.This ());
            args.GetReturnValue ().Set (args.This ());
        }
        else
        {
            auto cons = args.Data ().As<Object> ()->GetInternalField (0).As<Function> ();
            auto result = cons->NewInstance (context, 0, nullptr).ToLocalChecked ();
            args.GetReturnValue ().Set (result);
        }
    }

    static void Start (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        nodeFileSaver->fileSaver.start ();
    }

    static void Stop (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        nodeFileSaver->fileSaver.stop ();
    }

    static void Scan (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate ();
        auto context = isolate->GetCurrentContext ();
        auto target = args[0]->ToString (context);
        if (target.IsEmpty ())
        {
            isolate->ThrowException (Exception::TypeError (
                String::NewFromUtf8 (isolate, "Missing required target argument", NewStringType::kNormal)
                    .ToLocalChecked ()));
            return;
        }

        std::string output;
        output.resize (static_cast<unsigned long> (target.ToLocalChecked ()->Length ()));
        target.ToLocalChecked ()->WriteUtf8 (isolate, output.data ());
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        nodeFileSaver->fileSaver.scan (output);
    }

    static void GetCurrentSizeAt (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate ();
        auto context = isolate->GetCurrentContext ();
        auto target = args[0]->ToString (context);
        if (target.IsEmpty ())
        {
            isolate->ThrowException (Exception::TypeError (
                String::NewFromUtf8 (isolate, "Missing required target argument", NewStringType::kNormal)
                    .ToLocalChecked ()));
            return;
        }

        std::string output;
        output.resize (static_cast<unsigned long> (target.ToLocalChecked ()->Length ()));
        target.ToLocalChecked ()->WriteUtf8 (isolate, output.data ());
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getCurrentSizeAt (output);

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetTargets (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate ();
        auto context = isolate->GetCurrentContext ();
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getTargets ();

        auto resultArray = v8::Array::New (isolate, static_cast<int> (result.size ()));
        unsigned int i = 0;
        for (const auto& pth : result)
        {
            auto value =
                String::NewFromUtf8 (isolate, pth.string ().c_str (), NewStringType::kNormal).ToLocalChecked ();
            auto r = resultArray->Set (context, i, value);

            if (!r.FromMaybe (false))
            {
                isolate->ThrowException (Exception::Error (
                    String::NewFromUtf8 (isolate, "Failed to create result array", NewStringType::kNormal)
                        .ToLocalChecked ()));
            }

            i += 1;
        }

        args.GetReturnValue ().Set (resultArray);
    }

    static void GetTotalFiles (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getTotalFiles ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetTotalKnownFiles (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getTotalKnownFiles ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetFilesPerSecond (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getFilesPerSecond ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetNumWorkers (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getNumWorkers ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetElapsed (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getElapsed ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetStorageQueueSize (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getStorageQueueSize ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void GetInMemoryEntryCount (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.getInMemoryEntryCount ();

        args.GetReturnValue ().Set (static_cast<double> (result));
    }

    static void IsPathFinished (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate ();
        auto context = isolate->GetCurrentContext ();
        auto target = args[0]->ToString (context);
        if (target.IsEmpty ())
        {
            isolate->ThrowException (Exception::TypeError (
                String::NewFromUtf8 (isolate, "Missing required target argument", NewStringType::kNormal)
                    .ToLocalChecked ()));
            return;
        }

        std::string output;
        output.resize (static_cast<unsigned long> (target.ToLocalChecked ()->Length ()));
        target.ToLocalChecked ()->WriteUtf8 (isolate, output.data ());
        boost::filesystem::path outputPath{output};

        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.isPathFinished (outputPath);

        args.GetReturnValue ().Set (static_cast<bool> (result));
    }

    static void AreAllTargetsFinished (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        auto result = nodeFileSaver->fileSaver.areAllTargetsFinished ();

        args.GetReturnValue ().Set (static_cast<bool> (result));
    }

    static void SetupDefaultStorage (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        nodeFileSaver->fileSaver.setupDefaultStorage ();
    }

    static void SetNumWorkers (const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate ();
        auto context = isolate->GetCurrentContext ();
        auto numWorkers = args[0]->ToUint32 (context);
        if (numWorkers.IsEmpty ())
        {
            isolate->ThrowException (Exception::TypeError (
                String::NewFromUtf8 (isolate, "Missing required numWorkers argument", NewStringType::kNormal)
                    .ToLocalChecked ()));
            return;
        }

        auto* nodeFileSaver = ObjectWrap::Unwrap<NodeFileSaver> (args.Holder ());
        nodeFileSaver->fileSaver.setNumWorkers (numWorkers.ToLocalChecked ()->Value ());
    }

    FileSaver fileSaver;
};

void initialize (Local<Object> exports)
{
    NodeFileSaver::initialize (exports);
}

NODE_MODULE (NODE_GYP_MODULE_NAME, initialize)

} // namespace filesaver::node_addon
