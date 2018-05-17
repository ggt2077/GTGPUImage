#import <UIKit/UIKit.h>
#import "GPUImageOutput.h"

// 从名称就可以知道GPUImagePicture是GPUImage框架中处理与图片相关的类,它的主要作用是将UIImage或CGImage转化为纹理对象。GPUImagePicture继承自GPUImageOutput,从而可以知道它能够作为输出,由于它没有实现GPUImageInput协议,不能处理输入。因此,常常作为响应链源
@interface GPUImagePicture : GPUImageOutput
{
    CGSize pixelSizeOfImage;
    BOOL hasProcessedImage;
    
    dispatch_semaphore_t imageUpdateSemaphore;
}

// 初始化方法比较多,但是其它初始化方法都是基于下面的这个初始化方法。因此,只看下面的这个初始化方法。实现比较复杂,但是基本思路就是:1、获取图片适合的宽高(不能超出OpenGL ES允许的最大纹理宽高)2、如果使用了smoothlyScaleOutput,需要调整宽高为接近2的幂的值,调整后必须重绘;3、如果不用重绘,则获取大小端、alpha等信息;4、需要重绘,则使用CoreGraphics重绘;5、根据是否需要去除预乘alpha选项,决定是否去除预乘alpha;6、由调整后的数据生成纹理缓存对象;7、根据shouldSmoothlyScaleOutput选项决定是否生成mipmaps;8、最后释放资源。

// Initialization and teardown
// 通过图片URL初始化
- (id)initWithURL:(NSURL *)url;
// 通过UIImage或CGImage初始化
- (id)initWithImage:(UIImage *)newImageSource;
- (id)initWithCGImage:(CGImageRef)newImageSource;
// 通过UIImage或CGImage、是否平滑缩放输出来初始化
- (id)initWithImage:(UIImage *)newImageSource smoothlyScaleOutput:(BOOL)smoothlyScaleOutput;
- (id)initWithCGImage:(CGImageRef)newImageSource smoothlyScaleOutput:(BOOL)smoothlyScaleOutput;

// Image rendering
// 处理图片
- (void)processImage;
// 输出图片大小,由于图像大小可能被调整(详见初始化方法)。因此,提供了获取图像大小的方法。
- (CGSize)outputImageSize;

/**
 * Process image with all targets and filters asynchronously
 * The completion handler is called after processing finished in the
 * GPU's dispatch queue - and only if this method did not return NO.
 *
 * @returns NO if resource is blocked and processing is discarded, YES otherwise
 */
- (BOOL)processImageWithCompletionHandler:(void (^)(void))completion;
- (void)processImageUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withCompletionHandler:(void (^)(UIImage *processedImage))block;

@end
