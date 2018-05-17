#import "GPUImageVideoCamera.h"

void stillImageDataReleaseCallback(void *releaseRefCon, const void *baseAddress);
void GPUImageCreateResizedSampleBuffer(CVPixelBufferRef cameraFrame, CGSize finalSize, CMSampleBufferRef *sampleBuffer);

/**
 GPUImageStillCamera主要用来进行拍照。它继承自 GPUImageVideoCamera,因此,除了具备GPUImageVideoCamera的功能,它还提供了一套丰富的拍照API,方便我们进行拍照的相关操作。
 */
@interface GPUImageStillCamera : GPUImageVideoCamera

/** The JPEG compression quality to use when capturing a photo as a JPEG.
 // jpeg图片的压缩率,默认是0.8
 */
@property CGFloat jpegCompressionQuality;

// Only reliably set inside the context of the completion handler of one of the capture methods
// 图片的Metadata信息
@property (readonly) NSDictionary *currentCaptureMetadata;

// 方法列表。方法主要是和拍照相关,输出的类型比较丰富,可以是CMSampleBuffer、UIImage、NSData 等。如果有滤镜需要,还可以传入相关滤镜(final Filter In Chain)。
// 虽然API比较丰富,但是最终都是调用的私有方法 - (void)capturePhotoProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withImageOnGPUHandler:(void (^)(NSError *error))block。因此,着重留意该方法就行了。

// Photography controls
- (void)capturePhotoAsSampleBufferWithCompletionHandler:(void (^)(CMSampleBufferRef imageSampleBuffer, NSError *error))block;
- (void)capturePhotoAsImageProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withCompletionHandler:(void (^)(UIImage *processedImage, NSError *error))block;
- (void)capturePhotoAsImageProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withOrientation:(UIImageOrientation)orientation withCompletionHandler:(void (^)(UIImage *processedImage, NSError *error))block;
- (void)capturePhotoAsJPEGProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withCompletionHandler:(void (^)(NSData *processedJPEG, NSError *error))block;
- (void)capturePhotoAsJPEGProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withOrientation:(UIImageOrientation)orientation withCompletionHandler:(void (^)(NSData *processedJPEG, NSError *error))block;
- (void)capturePhotoAsPNGProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withCompletionHandler:(void (^)(NSData *processedPNG, NSError *error))block;
- (void)capturePhotoAsPNGProcessedUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain withOrientation:(UIImageOrientation)orientation withCompletionHandler:(void (^)(NSData *processedPNG, NSError *error))block;

@end
