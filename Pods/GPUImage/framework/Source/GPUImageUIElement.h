#import "GPUImageOutput.h"

/**
 与GPUImagePicture类似可以作为响应链源。与GPUImagePicture不同的是,它的数据不是来自图片,而是来自于UIView或CALayer的渲染结果,类似于对UIView或CALayer截图。GPUImageUIElement继承自GPUImageOutput,从而可以知道它能够作为输出,由于它没有实现GPUImageInput协议,不能处理输入。
 */
@interface GPUImageUIElement : GPUImageOutput

// Initialization and teardown
// 通过UIView或CALayer进行初始化,初始化的过程中调用 [layer renderInContext:imageContext] 进行渲染,渲染后便生成纹理对象。
- (id)initWithView:(UIView *)inputView;
- (id)initWithLayer:(CALayer *)inputLayer;

// Layer management 下列方法主要是与截屏生成纹理对象并传给所有targets处理相关
// 获取像素大小
- (CGSize)layerSizeInPixels;
// 更新方法
- (void)update;
// 使用当前时间的更新方法
- (void)updateUsingCurrentTime;
// 带时间的更新方法
- (void)updateWithTimestamp:(CMTime)frameTime;

@end
