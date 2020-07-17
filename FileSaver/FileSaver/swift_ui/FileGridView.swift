//
//  FileGridView.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 17/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import AppKit
import SwiftUI

struct FileGridView: NSViewControllerRepresentable {
    typealias NSViewControllerType = NSViewController

    func makeNSViewController(context: Context) -> NSViewController {
        return NSStoryboard(name: "File Browser", bundle: nil).instantiateInitialController()!
    }

    func updateNSViewController(_ nsViewController: NSViewController, context: Context) {
    }
}

struct FileGridView_Previews: PreviewProvider {
    static var previews: some View {
        FileGridView()
    }
}
