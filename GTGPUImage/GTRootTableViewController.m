//
//  GTRootTableViewController.m
//  GTGPUImage
//
//  Created by Tony on 2018/5/17.
//  Copyright © 2018年 58. All rights reserved.
//

#import "GTRootTableViewController.h"
#import "VideoViewController.h"
#import "PhotoViewController.h"

@interface GTRootTableViewController ()

@property (nonatomic, strong) NSArray *dataSource;

@end

@implementation GTRootTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    _dataSource = @[
                    @{@"视频录制": NSStringFromClass([VideoViewController class])},
                    @{@"拍摄照片": NSStringFromClass([PhotoViewController class])}
                    ];
    [self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:NSStringFromClass([UITableViewCell class])];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return _dataSource.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:NSStringFromClass([UITableViewCell class]) forIndexPath:indexPath];
    NSDictionary *dic = [self.dataSource objectAtIndex:indexPath.row];
    cell.textLabel.text = [dic keyEnumerator].allObjects.firstObject;
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSDictionary *dic = [self.dataSource objectAtIndex:indexPath.row];
    NSString *classStr = [dic allValues].firstObject;
    Class vcClass = NSClassFromString(classStr);
    UIViewController *controller = (UIViewController *)[[vcClass alloc] init];
    [self.navigationController pushViewController:controller animated:YES];
}

@end
