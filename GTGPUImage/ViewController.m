//
//  ViewController.m
//  GTGPUImage
//
//  Created by Tony on 2018/5/15.
//  Copyright © 2018年 58. All rights reserved.
//

#import "ViewController.h"
#import <GPUImage/GPUImage.h>

#define DOCUMENT(path) [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:path]

@interface ViewController ()

@property (weak, nonatomic) IBOutlet GPUImageView *imageView;

@property (nonatomic, strong) GPUImageVideoCamera *video;

@property (nonatomic, strong) GPUImageMovieWriter *writer;

@property (nonatomic, strong) NSURL *videoFile;

@property (nonatomic, assign, readonly, getter=isRecording) BOOL recording;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    _recording = NO;
    // 设置背景色
    [_imageView setBackgroundColorRed:1.0 green:1.0 blue:1.0 alpha:1.0];
    // 设置保存文件路径
    _videoFile = [NSURL fileURLWithPath:DOCUMENT(@"/1.mov")];
    // 删除文件
    [[NSFileManager defaultManager] removeItemAtURL:_videoFile error:nil];
    // 设置GPUImageMovieWriter
    _writer = [[GPUImageMovieWriter alloc] initWithMovieURL:_videoFile size:self.view.bounds.size];
    [_writer setHasAudioTrack:YES audioSettings:nil];
    // 设置GPUImageVideoCamera
    _video = [[GPUImageVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPreset640x480  cameraPosition:AVCaptureDevicePositionBack];
    _video.outputImageOrientation = UIInterfaceOrientationPortrait;
    [_video addAudioInputsAndOutputs];
    // 设置音频处理target
    _video.audioEncodingTarget = _writer;
    // 设置target
    [_video addTarget:_imageView];
    [_video addTarget:_writer];
    // 开始拍摄
    [_video startCameraCapture]; 
}

@end
