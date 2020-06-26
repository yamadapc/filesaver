//
//  MainFileTablePage.swift
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 27/6/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

import Cocoa

@objc
public class MainFileTablePageController: NSSplitViewController {

    public override func viewDidLoad() {
        super.viewDidLoad()

        let viewLeft: NSSplitViewItem = NSSplitViewItem()
        self.addSplitViewItem(viewLeft)
    }
    
}
