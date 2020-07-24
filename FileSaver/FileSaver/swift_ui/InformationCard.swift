//
//  InformationCard.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 24/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import SwiftUI

struct InformationCard: View {
    let label: String
    let value: String

    var body: some View {
        Card {
            VStack(alignment: .leading, spacing: 7.5) {
                Text(self.label)
                Text(self.value)
                    .font(Font.system(size: 15.0, weight: .bold, design: .default))
            }
            .frame(alignment: .leading)
        }
        .frame(alignment: .leading)
    }
}

struct InformationCard_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            InformationCard(label: "Files per second", value: "25.000")
                .padding(15.0)
                .frame(minWidth: 0.0, minHeight: 0.0)
                .fixedSize()
                .previewDisplayName("Simple")

            InformationCard(label: "Files per second", value: "25.000")
                .padding(15.0)
                .frame(minWidth: 0.0, minHeight: 0.0)
                .previewDisplayName("Simple")
        }
    }
}
