//
//  SecondViewController.m
//  GTGPUImage
//
//  Created by Tony on 2018/5/17.
//  Copyright © 2018年 58. All rights reserved.
//

#import "PhotoViewController.h"
#import <GPUImage/GPUImage.h>
#import "GTMacros.h"

@interface PhotoViewController ()

@property (strong, nonatomic) GPUImageView *imageView;
@property (nonatomic, strong) GPUImageStillCamera *camera;
@property (nonatomic, strong) GPUImageFilter *filter;
@property (nonatomic, strong) UIButton *takePhotoButton;
@property (nonatomic, strong) UIImageView *photoView;

@end

@implementation PhotoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.view addSubview:self.imageView];
    
    self.takePhotoButton.frame = CGRectMake(0, 0, 100, 50);
    self.takePhotoButton.center = self.view.center;
    [self.view addSubview:self.takePhotoButton];
    
    self.photoView.frame = CGRectMake(0, self.view.bounds.size.height - 100, 100, 100);
    [self.view addSubview:self.photoView];
    
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

- (void)pictureButtonTapped:(UIButton *)sender
{
    if ([_camera.captureSession isRunning]) {
        [_camera capturePhotoAsImageProcessedUpToFilter:_filter withCompletionHandler:^(UIImage *processedImage, NSError *error) {
            [_camera stopCameraCapture];
            if (!error) {
                [_photoView setImage:processedImage];
            }
        }];
    } else {
        [_camera startCameraCapture];
    }
}

#pragma mark -- Getter --

- (GPUImageView *)imageView {
    if (!_imageView) {
        _imageView = [[GPUImageView alloc] initWithFrame:self.view.bounds];
    }
    return _imageView;
}

- (UIButton *)takePhotoButton {
    if (!_takePhotoButton) {
        _takePhotoButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_takePhotoButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        _takePhotoButton.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.6];
        _takePhotoButton.titleLabel.font = [UIFont systemFontOfSize:30];
        _takePhotoButton.layer.borderColor = [UIColor whiteColor].CGColor;
        _takePhotoButton.layer.borderWidth = 0.5;
        _takePhotoButton.layer.cornerRadius = 2.0;
        _takePhotoButton.layer.masksToBounds= YES;
        [_takePhotoButton setTitle:@"拍摄" forState:UIControlStateNormal];
        [_takePhotoButton addTarget:self action:@selector(pictureButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _takePhotoButton;
}

- (UIImageView *)photoView {
    if (!_photoView) {
        _photoView = [[UIImageView alloc] init];
    }
    return _photoView;
}

@end
