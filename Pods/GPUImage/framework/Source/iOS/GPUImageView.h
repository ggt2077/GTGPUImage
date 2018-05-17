#import <UIKit/UIKit.h>
#import "GPUImageContext.h"

typedef enum {
    kGPUImageFillModeStretch,                       // Stretch to fill the full view, which may distort the image outside of its normal aspect ratio
    kGPUImageFillModePreserveAspectRatio,           // Maintains the aspect ratio of the source image, adding bars of the specified background color
    kGPUImageFillModePreserveAspectRatioAndFill     // Maintains the aspect ratio of the source image, zooming in on its center to fill the view
} GPUImageFillModeType; 

/**
 UIView subclass to use as an endpoint for displaying GPUImage outputs
 从名称就可以知道GPUImageView是GPUImage框架中显示图片相关的类。GPUImageView实现了GPUImageInput协议,从而可以知道它能够接受GPUImageFramebuffer的输入。因此,常常作为响应链的终端节点,用于显示处理后的帧缓存。GPUImageView这个涉及了比较多的OpenGL ES的知识,在这里不会说太多OpenGL ES的知识。如果不太了解,欢迎阅读我的 OpenGL ES入门专题。
 */
@interface GPUImageView : UIView <GPUImageInput>
{
    GPUImageRotationMode inputRotation;
}

/** The fill mode dictates how images are fit in the view, with the default being kGPUImageFillModePreserveAspectRatio
 */
@property(readwrite, nonatomic) GPUImageFillModeType fillMode;

/** This calculates the current display size, in pixels, taking into account Retina scaling factors
 */
@property(readonly, nonatomic) CGSize sizeInPixels;

@property(nonatomic) BOOL enabled;

/** Handling fill mode
 
 @param redComponent Red component for background color
 @param greenComponent Green component for background color
 @param blueComponent Blue component for background color
 @param alphaComponent Alpha component for background color
 // 设置背景颜色
 */
- (void)setBackgroundColorRed:(GLfloat)redComponent green:(GLfloat)greenComponent blue:(GLfloat)blueComponent alpha:(GLfloat)alphaComponent;

- (void)setCurrentlyReceivingMonochromeInput:(BOOL)newValue;

@end
