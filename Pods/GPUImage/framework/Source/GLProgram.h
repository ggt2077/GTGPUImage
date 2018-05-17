//  This is Jeff LaMarche's GLProgram OpenGL shader wrapper class from his OpenGL ES 2.0 book.
//  A description of this can be found at his page on the topic:
//  http://iphonedevelopment.blogspot.com/2010/11/opengl-es-20-for-ios-chapter-4.html
//  I've extended this to be able to take programs as NSStrings in addition to files, for baked-in shaders

#import <Foundation/Foundation.h>

#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#endif

/**
 之前在学习OpenGL的时候,我们首先要需要做的就是初始化OpenGL ES环境,编译、链接顶点着色器和片元着色器。在GPUImage中封装了GLProgram专门处理OpenGL ES程序的创建等相关工作。
 */
@interface GLProgram : NSObject
{
    NSMutableArray  *attributes;
    NSMutableArray  *uniforms;
    GLuint          program,
	vertShader, 
	fragShader;	
}

@property(readwrite, nonatomic) BOOL initialized;
@property(readwrite, copy, nonatomic) NSString *vertexShaderLog;
@property(readwrite, copy, nonatomic) NSString *fragmentShaderLog;
@property(readwrite, copy, nonatomic) NSString *programLog;

// 初始化方法,可以根据需要传入顶点着色器的路径或字符串以及片源着色器的路径及字符串进行初始化。
// 初始化的过程包含了顶点片源着色器的创建、编译,着色器程序的创建,顶点片源着色器附着到着色器程序等过程。
- (id)initWithVertexShaderString:(NSString *)vShaderString 
            fragmentShaderString:(NSString *)fShaderString;
- (id)initWithVertexShaderString:(NSString *)vShaderString 
          fragmentShaderFilename:(NSString *)fShaderFilename;
- (id)initWithVertexShaderFilename:(NSString *)vShaderFilename 
            fragmentShaderFilename:(NSString *)fShaderFilename;
- (void)addAttribute:(NSString *)attributeName;
- (GLuint)attributeIndex:(NSString *)attributeName;
- (GLuint)uniformIndex:(NSString *)uniformName;
- (BOOL)link;
- (void)use;
- (void)validate;
@end
