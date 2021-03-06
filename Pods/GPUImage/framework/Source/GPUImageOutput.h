#import "GPUImageContext.h"
#import "GPUImageFramebuffer.h"

#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#else
// For now, just redefine this on the Mac
typedef NS_ENUM(NSInteger, UIImageOrientation) {
    UIImageOrientationUp,            // default orientation
    UIImageOrientationDown,          // 180 deg rotation
    UIImageOrientationLeft,          // 90 deg CCW
    UIImageOrientationRight,         // 90 deg CW
    UIImageOrientationUpMirrored,    // as above but image mirrored along other axis. horizontal flip
    UIImageOrientationDownMirrored,  // horizontal flip
    UIImageOrientationLeftMirrored,  // vertical flip
    UIImageOrientationRightMirrored, // vertical flip
};
#endif

void runOnMainQueueWithoutDeadlocking(void (^block)(void));
void runSynchronouslyOnVideoProcessingQueue(void (^block)(void));
void runAsynchronouslyOnVideoProcessingQueue(void (^block)(void));
void runSynchronouslyOnContextQueue(GPUImageContext *context, void (^block)(void));
void runAsynchronouslyOnContextQueue(GPUImageContext *context, void (^block)(void));
void reportAvailableMemoryForGPUImage(NSString *tag);

@class GPUImageMovieWriter;

/** GPUImage's base source object
 
 Images or frames of video are uploaded from source objects, which are subclasses of GPUImageOutput. These include:
 
 - GPUImageVideoCamera (for live video from an iOS camera) 
 - GPUImageStillCamera (for taking photos with the camera)
 - GPUImagePicture (for still images)
 - GPUImageMovie (for movies)
 
 Source objects upload still image frames to OpenGL ES as textures, then hand those textures off to the next objects in the processing chain.
 GPUImageOutput 表示该类能够作为输出,输出的是 GPUImageFramebuffer 对象。该类的实现比较简单,主要是实现了一些最基本的方法,这些方法不需要依赖具体细节,细节处理在子类中完成。继承 GPUImageOutput 的类也比较多,比如:GPUImageFilter、GPUImageVideoCamera、GPUImageStillCamera、GPUImagePicture 等
 */
@interface GPUImageOutput : NSObject
{
    // 输出的帧缓存对象
    GPUImageFramebuffer *outputFramebuffer;
    // target列表,target纹理索引列表
    NSMutableArray *targets, *targetTextureIndices;
    // 纹理尺寸
    CGSize inputTextureSize, cachedMaximumOutputSize, forcedMaximumSize;
    
    BOOL overrideInputSize;
    
    BOOL allTargetsWantMonochromeData;
    // 设置下一帧提取图片
    BOOL usingNextFrameForImageCapture;
}
// 是否使用mipmaps
@property(readwrite, nonatomic) BOOL shouldSmoothlyScaleOutput;
// 是否忽略处理当前Target
@property(readwrite, nonatomic) BOOL shouldIgnoreUpdatesToThisTarget;
@property(readwrite, nonatomic, retain) GPUImageMovieWriter *audioEncodingTarget;
// 当前忽略处理的Target
@property(readwrite, nonatomic, unsafe_unretained) id<GPUImageInput> targetToIgnoreForUpdates;
// 每帧处理完回调
@property(nonatomic, copy) void(^frameProcessingCompletionBlock)(GPUImageOutput*, CMTime);
// 是否启用渲染目标
@property(nonatomic) BOOL enabled;
// 纹理选项
@property(readwrite, nonatomic) GPUTextureOptions outputTextureOptions;

/// 方法列表。GPUImageOutput 提供方法主要是以下几种类型:1、帧缓冲对象管理;2、响应链的管理;3、图像提取。
/// @name Managing targets
// 设置输入的帧缓冲对象以及纹理索引
- (void)setInputFramebufferForTarget:(id<GPUImageInput>)target atIndex:(NSInteger)inputTextureIndex;
// 输出的帧缓冲对象
- (GPUImageFramebuffer *)framebufferForOutput;
// 删除帧缓冲对象
- (void)removeOutputFramebuffer;
// 通知所有的Target
- (void)notifyTargetsAboutNewOutputTexture;

/** Returns an array of the current targets.
 所有的Target列表
 */
- (NSArray*)targets;

/** Adds a target to receive notifications when new frames are available.
 
 The target will be asked for its next available texture.
 
 See [GPUImageInput newFrameReadyAtTime:]
 
 @param newTarget Target to be added
 // 增加Target
 */
- (void)addTarget:(id<GPUImageInput>)newTarget;

/** Adds a target to receive notifications when new frames are available.
 
 See [GPUImageInput newFrameReadyAtTime:]
 
 @param newTarget Target to be added
 */
- (void)addTarget:(id<GPUImageInput>)newTarget atTextureLocation:(NSInteger)textureLocation;

/** Removes a target. The target will no longer receive notifications when new frames are available.
 
 @param targetToRemove Target to be removed
 删除Target
 */
- (void)removeTarget:(id<GPUImageInput>)targetToRemove;

/** Removes all targets.
 */
- (void)removeAllTargets;

/// @name Manage the output texture
// 强制按照传入的尺寸处理
- (void)forceProcessingAtSize:(CGSize)frameSize;
- (void)forceProcessingAtSizeRespectingAspectRatio:(CGSize)frameSize;

/// @name Still image processing
// 从帧缓冲对象提取CGImage图像
- (void)useNextFrameForImageCapture;
- (CGImageRef)newCGImageFromCurrentlyProcessedOutput;
// 使用静态图片做滤镜纹理
- (CGImageRef)newCGImageByFilteringCGImage:(CGImageRef)imageToFilter;

// Platform-specific image output methods
// If you're trying to use these methods, remember that you need to set -useNextFrameForImageCapture before running -processImage or running video and calling any of these methods, or you will get a nil image
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
// 从帧缓冲对象提取UIImage图像
- (UIImage *)imageFromCurrentFramebuffer;
- (UIImage *)imageFromCurrentFramebufferWithOrientation:(UIImageOrientation)imageOrientation;
// 使用静态图片做滤镜纹理
- (UIImage *)imageByFilteringImage:(UIImage *)imageToFilter;
- (CGImageRef)newCGImageByFilteringImage:(UIImage *)imageToFilter;
#else
- (NSImage *)imageFromCurrentFramebuffer;
- (NSImage *)imageFromCurrentFramebufferWithOrientation:(UIImageOrientation)imageOrientation;
- (NSImage *)imageByFilteringImage:(NSImage *)imageToFilter;
- (CGImageRef)newCGImageByFilteringImage:(NSImage *)imageToFilter;
#endif
// 是否提供单色输出
- (BOOL)providesMonochromeOutput;

@end
