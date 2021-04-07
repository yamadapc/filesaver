//
//  StatisticsPanel.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 16/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import SwiftUI

struct StatisticsPanel: View {
    @ObservedObject var model: StatisticsPanelModel
    var controller: StatisticsPoolingController

    init() {
        let model = StatisticsPanelModel()
        self.model = model
        self.controller = StatisticsPoolingController(statisticsPanelModel: model)
    }

    var body: some View {
        Group {
            VStack(alignment: .center, spacing: 15.0) {
                HStack(alignment: .center, spacing: 15.0) {
                    InformationCard(
                        label: "Files per second",
                        value: renderFilesPerSecond(filesPerSecond: self.model.filesPerSecond)
                    )
                    InformationCard(
                        label: "Total files",
                        value: self.model.totalFiles.description
                    )
                }
                .frame(maxHeight: 80)

                Card {
                    Text("Charts")
                }
                .frame(maxHeight: .infinity)
            }
        }
        .padding(15.0)
        .background(
            Rectangle()
                .fill(Color(NSColor.windowBackgroundColor))
        )
    }
}

struct StatisticsPanel_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            StatisticsPanel()
                .frame(width: 500.0, height: 300.0, alignment: .leading)
                .previewDisplayName("Light mode")
                .environment(\.colorScheme, .light)

            StatisticsPanel()
                .frame(width: 500.0, height: 300.0, alignment: .leading)
                .previewDisplayName("Dark mode")
                .environment(\.colorScheme, .dark)
        }

    }
}
