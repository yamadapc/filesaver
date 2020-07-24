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
            case "Files": return AnyView(FileGridView())
            case "Statistics": return AnyView(StatisticsPanel())
            default: return AnyView(Text("Not implemented"))
        }
    }

    var body: some View {
        HStack {
            getContent()
        }
        .frame(
            idealWidth: 500.0,
            maxWidth: .infinity,
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
