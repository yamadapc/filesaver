//
//  ContentView.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 16/7/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        HStack {
            Text("Hello, World!")
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
        ContentView()
    }
}
