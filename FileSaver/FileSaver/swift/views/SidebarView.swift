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
                .padding(EdgeInsets(top: 5.0, leading: 10.0, bottom: 5.0, trailing: 10.0))
                .frame(maxWidth: .infinity, alignment: .leading)
                .background(
                    Rectangle()
                        .fill(
                            Color.secondary.opacity(
                                item.isActive ? 0.1 : 0.01
                            )
                        )
                        .cornerRadius(8.0)
                        .padding(3.0)
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

class NavigationState: ObservableObject {
    var values: [NavigationItem]
    @Published var activeItem: NavigationItem

    init(values: [NavigationItem]) {
        self.values = values
        let activeItemIndex = values.firstIndex(where: { item in item.isActive }) ?? 0
        self.activeItem = values[activeItemIndex]
    }

    func setActive(_ item: NavigationItem) {
        for otherItem in self.values {
            otherItem.isActive = false
        }
        item.isActive = true
        self.activeItem = item
    }
}

struct SidebarView: View {
    let state: NavigationState

    func onClick(_ item: NavigationItem) {
        state.setActive(item)
    }

    var body: some View {
        HStack(spacing: 0) {
            ScrollView(showsIndicators: false) {
                VStack(alignment: .leading, spacing: 0.0) {
                    ForEach(state.values, id: \.self.name) { item in
                        NavigationItemView(item: item, onClick: self.onClick)
                    }
                }
                .frame(minWidth: 150, maxWidth: .infinity)
            }
            .frame(minWidth: 150, maxWidth: 200)
            .visualEffect(material: .underWindowBackground)
        }
    }
}

struct SidebarView_Previews: PreviewProvider {
    static let defaultMenuItems = NavigationState(values: [
        NavigationItem(name: "Files", image: NSImage(named: NSImage.touchBarFolderTemplateName)!),
        NavigationItem(name: "Statistics", image: NSImage(named: NSImage.touchBarBookmarksTemplateName)!),
    ])
    static let defaultMenuItemsWithActive = NavigationState(values: [
        NavigationItem(name: "Files", image: NSImage(named: NSImage.touchBarFolderTemplateName)!),
        NavigationItem(name: "Statistics", image: NSImage(named: NSImage.touchBarBookmarksTemplateName)!, isActive: true),
    ])

    static var previews: some View {
        Group {
            SidebarView(state: defaultMenuItems)
                .previewDisplayName("Default sidebar")
            SidebarView(state: defaultMenuItemsWithActive)                .previewDisplayName("Active item sidebar")
        }
    }
}
