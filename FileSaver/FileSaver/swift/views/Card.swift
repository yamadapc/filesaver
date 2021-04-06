//
//  Card.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 22/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import SwiftUI

struct Card<Content: View>: View {
    let builder: () -> Content

    var body: some View {
        ZStack {
            RoundedRectangle(cornerRadius: 15, style: .continuous)
                .fill(Color(NSColor.controlBackgroundColor))
                .shadow(color: Color.black.opacity(0.15), radius: 3.0, y: 2)
                .frame(minWidth: 0.0, minHeight: 0.0, alignment: .leading)

            Group {
                builder()
            }
            .frame(alignment: .leading)
            .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .leading)
            .padding(15.0)
        }
    }
}

struct Card_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            Card {
                Text("Hello")
            }
            .frame(width: 300.0, height: 300.0, alignment: .leading)
            .padding(20.0)
            .previewDisplayName("Light mode")
            .environment(\.colorScheme, .light)

            Card {
                Text("Hello")
            }
            .frame(width: 300.0, height: 300.0, alignment: .leading)
            .padding(20.0)
            .previewDisplayName("Dark mode")
            .environment(\.colorScheme, .dark)
        }
    }
}
