//
//  MainUIHostingView.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 16/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import AppKit
import SwiftUI

class MainUIHostingViewController: NSHostingController<MainUI> {
    required init?(coder: NSCoder) {
        super.init(coder: coder, rootView: MainUI())
    }
}

struct MainUI: View {
    @State var menuItems = [
        NavigationItem(name: "Files", image: NSImage(named: NSImage.touchBarFolderTemplateName)!, isActive: true),
        NavigationItem(name: "Statistics", image: NSImage(named: NSImage.touchBarBookmarksTemplateName)!),
    ]

    var body: some View {
        NavigationView {
            SidebarView(menuItems: menuItems)
            ContentView()
        }.navigationViewStyle(DoubleColumnNavigationViewStyle())
    }
}

struct MainUI_Previews: PreviewProvider {
    static var previews: some View {
        MainUI(
            menuItems: [
                NavigationItem(name: "Files", image: NSImage(named: NSImage.touchBarFolderTemplateName)!),
                NavigationItem(name: "Statistics", image: NSImage(named: NSImage.touchBarBookmarksTemplateName)!),
            ]
        )
    }
}
