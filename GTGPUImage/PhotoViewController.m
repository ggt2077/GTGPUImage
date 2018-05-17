//
//  SecondViewController.m
//  GTGPUImage
//
//  Created by Tony on 2018/5/17.
//  Copyright © 2018年 58. All rights reserved.
//

#import "PhotoViewController.h"
#import <GPUImage/GPUImage.h>

@interface PhotoViewController ()

@property (strong, nonatomic) GPUImageView *imageView;
@property (nonatomic, strong) GPUImageStillCamera *camera;
@property (nonatomic, strong) GPUImageFilter *filter;

@end

@implementation PhotoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.view addSubview:self.imageView];
    
    // 设置背景色
    [_imageView setBackgroundColorRed:1.0 green:1.0 blue:1.0 alpha:1.0];
    // 滤镜
    _filter = [[GPUImageGrayscaleFilter alloc] init];
    // 初始化
    _camera = [[GPUImageStillCamera alloc] initWithSessionPreset:AVCaptureSessionPresetPhoto cameraPosition:AVCaptureDevicePositionBack];
    _camera.outputImageOrientation = UIInterfaceOrientationPortrait;
    
    [_camera addTarget:_imageView];
    [_camera addTarget:_filter];
    
    [_camera startCameraCapture];
}

- (GPUImageView *)imageView {
    if (!_imageView) {
        _imageView = [[GPUImageView alloc] initWithFrame:self.view.bounds];
    }
    return _imageView;
}

@end
