//
//  StatisticsPoolingController.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 6/4/21.
//  Copyright © 2021 Pedro Tacla Yamada. All rights reserved.
//

import Foundation

class StatisticsPoolingController {
    private var statisticsPanelModel: StatisticsPanelModel
    private var timer: Timer?

    init(
        statisticsPanelModel: StatisticsPanelModel
    ) {
        self.statisticsPanelModel = statisticsPanelModel
        self.start()
    }

    deinit {
        timer?.invalidate()
    }

    func start() {
        self.pool()
        self.timer = Timer.scheduledTimer(
            timeInterval: 1,
            target: self,
            selector: #selector(pool),
            userInfo: nil,
            repeats: true
        )
    }

    @objc func pool() {
        let filesPerSecond = ObjCFileSaver.getFilesPerSecond()
        let totalFiles = ObjCFileSaver.getTotalFiles()
        statisticsPanelModel.filesPerSecond = filesPerSecond
        statisticsPanelModel.totalFiles = totalFiles
    }
}
