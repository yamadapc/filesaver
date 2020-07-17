//
//  ContentView.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 16/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    var activeItem: NavigationItem

    func getContent() -> AnyView {
        switch activeItem.name {
            case "Files": return AnyView(Text("Files grid"))
            case "Statistics": return AnyView(Text("Not implemented"))
            default: return AnyView(Text("Not implemented"))
        }
    }

    var body: some View {
        HStack {
            getContent()
        }.frame(
            minWidth: 500,
            maxWidth: .infinity,
            minHeight: 500,
            maxHeight: .infinity,
            alignment: .center
        )
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView(
            activeItem: NavigationItem(
                name: "Files",
                image: NSImage(named: NSImage.touchBarAddTemplateName)!
            )
        )
    }
}