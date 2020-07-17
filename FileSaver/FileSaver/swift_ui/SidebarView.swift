//
//  SidebarView.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 16/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import AppKit
import SwiftUI

class NavigationItem: ObservableObject {
    let name: String
    let image: NSImage
    @Published var isActive: Bool

    init(name: String, image: NSImage) {
        self.name = name
        self.image = image
        self.isActive = false
    }

    init(name: String, image: NSImage, isActive: Bool) {
        self.name = name
        self.image = image
        self.isActive = isActive
    }
}

struct NavigationItemView: View {
    @ObservedObject var item: NavigationItem
    let onClick: (_ item: NavigationItem) -> Void

    var body: some View {
        Group {
            Button(action: { self.onClick(self.item) }) {
                HStack {
                    Image(nsImage: item.image)
                    Text(item.name).bold()
                }
                .padding(EdgeInsets(top: 10.0, leading: 15.0, bottom: 10.0, trailing: 15.0))
                .frame(maxWidth: .infinity, alignment: .leading)
                .background(
                    Rectangle()
                        .fill(
                            Color.secondary.opacity(
                                item.isActive ? 0.1 : 0.01
                            )
                        )
                        .cornerRadius(8.0)
                        .padding(5.0)
                )
            }
            .buttonStyle(PlainButtonStyle())
            .frame(maxWidth: .infinity, alignment: .leading)

            Rectangle()
                .fill(Color.secondary.opacity(0.4))
                .frame(maxWidth: .infinity, maxHeight: 1.0)
        }
    }
}

struct SidebarView: View {
    let menuItems: [NavigationItem]

    func onClick(_ item: NavigationItem) {
        for otherItem in menuItems {
            otherItem.isActive = false
        }
        item.isActive = true
    }

    var body: some View {
        HStack(spacing: 0) {
            ScrollView(showsIndicators: false) {
                VStack(alignment: .leading, spacing: 0.0) {
                    ForEach(menuItems, id: \.self.name) { item in
                        NavigationItemView(item: item, onClick: self.onClick)
                    }
                }
                .frame(minWidth: 200, maxWidth: .infinity)
            }
            .frame(minWidth: 200, maxWidth: .infinity)
            .visualEffect(material: .underWindowBackground)
        }
    }
}

struct SidebarView_Previews: PreviewProvider {
    static let defaultMenuItems = [
        NavigationItem(name: "Files", image: NSImage(named: NSImage.touchBarFolderTemplateName)!),
        NavigationItem(name: "Statistics", image: NSImage(named: NSImage.touchBarBookmarksTemplateName)!),
    ]
    static let defaultMenuItemsWithActive = [
        NavigationItem(name: "Files", image: NSImage(named: NSImage.touchBarFolderTemplateName)!),
        NavigationItem(name: "Statistics", image: NSImage(named: NSImage.touchBarBookmarksTemplateName)!, isActive: true),
    ]

    static var previews: some View {
        Group {
            SidebarView(menuItems: defaultMenuItems)
                .previewDisplayName("Default sidebar")
            SidebarView(menuItems: defaultMenuItemsWithActive)                .previewDisplayName("Active item sidebar")
        }
    }
}
