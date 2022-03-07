//
//  StatisticsPanelModel.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 6/4/21.
//  Copyright © 2021 Pedro Tacla Yamada. All rights reserved.
//

import Foundation
import Combine

class StatisticsPanelModel: ObservableObject {
    @Published var filesPerSecond: Double
    @Published var totalFiles: UInt
    @Published var speedHistory: [Double]

    init() {
        self.filesPerSecond = 0
        self.totalFiles = 0
        self.speedHistory = []
    }
}

func renderFilesPerSecond(filesPerSecond: Double) -> String {
    return String(format: "%.0f", arguments: [filesPerSecond])
}

let FILE_SIZE_SUFFIXES = [
    "B",
    "KB",
    "MB",
    "GB",
    "TB",
    "PB",
    "EB",
]

func renderBytes(bytes: Int) -> String {
    var suffixIndex = 0
    var count = Double(bytes)
    while (count >= 1024.0 && suffixIndex < 7) {
        suffixIndex += 1
        count /= 1024.0
    }

    if (count - floor(count) == 0.0) {
        return "\(count)\(FILE_SIZE_SUFFIXES[suffixIndex])"
    }

    return String(
        format: "%.1f%s",
        arguments: [
            count,
            FILE_SIZE_SUFFIXES[suffixIndex]
        ]
    )
}

func renderETA(time: Int) -> String {
    let seconds: Int = time / 1000
    let minutes: Int = seconds / 60
    let hours: Int = minutes / 60

    let renderedSeconds = seconds - (minutes * 60)
    let renderedMinutes = minutes - (hours * 60)
    let renderedHours = hours

    return String(
        format: "%d:%d:%d",
        arguments: [
            renderedHours,
            renderedMinutes,
            renderedSeconds
        ]
    )
}
