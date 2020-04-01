# iOS SDK

* 请先 cocoapods 集成方舟 SDK 易达专用版，或者手动下载：[AnalysysAgent-EasyTouch](https://github.com/analysys-ea/UBASDK)

* 易达 iOS SDK 下载：[动态库](https://github.com/analysys-ea/EASDK)、[静态库](https://github.com/analysys-ea/EASDK-StaticLib)

---

### 一、iOS SDK 简介

易达系统是易观基于方舟平台一款触达用户的产品，加强了产品的用户体验和提升了产品的转化率，易达的 iOS SDK 是此系统中重要的支撑点，它提供了：

* 提供触达用户的弹窗功能，支持弹窗样式：

* 图片样式，支持点击事件。

* 文本样式，支持标题、正文、两个按钮。

* 图文混合样式，最上面是图片，下面是标题、正文、按钮。

* H5 样式，支持 HTML 页面。

* 订阅事件，用户在后台如果创建了基于用户事件的触发行为（弹窗除外），SDK 就会在每次启动的时候根据是否有事件更新来拉取订阅列表，并在有订阅事件产生的时候通知易达系统。

#### iOS 版本支持

支持的 iOS 系统版本为 8.0 及以上版本。

#### 组成

SDK 有[动态库](https://github.com/analysys-ea/EASDK)和[静态库](https://github.com/analysys-ea/EASDK-StaticLib)两种版本，您可以根据自身需要选择其中一种集成即可。

* 动态库文件：

```
AnalysysEasyTouch.framework
```

* 静态库文件：

```
libea-ios-sdk-static.a
```

* 包含的头文件：

```
AnalysysEaManager.h
AnalysysEaConfig.h
```

#### 注意事项

请参考 iOS 常见问题

### 二、快速开始

#### 1、获取项目 AppKey

* 登录[易达系统](https://ea.analysys.cn/app.html#/Login)，创建项目，项目创建完成后自动生成对应的AppKey用以标识该项目（应用）。

#### 2、集成易达 SDK

**方式 1：cocoapods 导入动态库**

* 打开 Podfile 文件，添加如下代码

```
pod 'AnalysysEasyTouch' // 易达 SDK
```

* 如果需要安装指定版本，则按照以下方式

```
pod 'AnalysysEasyTouch', '1.1.0' // 示例版本号
```

* 特别注意：由于iOS 10以后苹果系统增加的 NSNotification Service Extension 扩展能够用于统计推送到达率，如果在 APP 中添加了该扩展而无法引入第三方的类文件，则需要使用以下“选择2”方式手动下载静态库并导入项目。将静态库及相关头文件添加到项目中的时候，需要同时勾选项目主 target 和 NSNotification Service Extension 扩展target，否则编译会报错。

**方式 2：手动下载静态库导入**

* [下载最新静态库 SDK](https://github.com/analysys-ea/EASDK-StaticLib)

* 解压缩后，拷贝 ea-ios-sdk-static 文件夹中的静态库.a文件和对应的.h头文件到项目中

* 选中项目 target ，在 Build Phases 》Link Binary With Libiaries 中检查对应的静态库.a文件是否已经添加，没有则需手动添加进去。

#### 3、添加头文件

* 如果使用的是动态库，在 AppDelegate.m 中引入以下头文件：

```
#import <AnalysysEasyTouch/AnalysysEaManager.h> // 易达 SDK
```

* 如果使用的是静态库，在 AppDelegate.m 中引入以下头文件：

```
#import "AnalysysEaManager.h" // 易达 SDK
```

#### 4、添加初始化代码

* 请将以下代码添加到 - \(BOOL\)application:\(UIApplication \*\)application didFinishLaunchingWithOptions:\(NSDictionary \*\)launchOptions

```
/*********** 易达 SDK 初始化 ***********/
AnalysysEaConfig *config = [AnalysysEaConfig defaultConfiguration];
config.appKey= @"易达后台创建项目的 AppKey";
[AnalysysEaManager startWithConfig:config];
```

* 通过方舟 SDK 上报相关用户属性，若您需要手机能收到 SDK 发送的短信、邮件等，需要在获取到手机号、邮箱的地方，比如登录的时候，调用以下方法：

```
// 上报用户手机号码
[AnalysysAgent profileSet:@"$PHONE" propertyValue:value];
// 上报用户邮箱
[AnalysysAgent profileSet:@"$EMAIL" propertyValue:value];
// 上报用户微信openID
[AnalysysAgent profileSet:@"$WECHATOPENID" propertyValue:value];
```

* 若您的 APP 支持远程推送功能，SDK 为您封装了苹果原生 APNS 远程推送注册接口

```
// 注册 APNS 远程推送
[AnalysysEasyTouch registerRemoteNotificationWithDelegate:self];
```
* 在成功注册推送并收到 deviceToken 的系统回调方法 - \(void\)application:\(UIApplication \*\)application didRegisterForRemoteNotificationsWithDeviceToken:\(NSData \*\)deviceToken 中上报解析后的 deviceToken

```
// iOS 13 之后的新的转换方法，兼容之前的版本
const unsigned *tokenBytes = [deviceToken bytes];
NSString *hexToken = [NSString stringWithFormat:@"%08x%08x%08x%08x%08x%08x%08x%08x",
ntohl(tokenBytes[0]), ntohl(tokenBytes[1]), ntohl(tokenBytes[2]),
ntohl(tokenBytes[3]), ntohl(tokenBytes[4]), ntohl(tokenBytes[5]),
ntohl(tokenBytes[6]), ntohl(tokenBytes[7])];
NSLog(@"\n>>>[DeviceToken Success]:%@\n\n", hexToken);

// 上报pushId（解析后的deviceToken）
// 目前易达 iOS SDK 只支持苹果 APNS 推送通道
[AnalysysEaManager registerDeviceToken:hexToken];
```

* 若 APP 支持推送功能，在收到推送及点击推送的系统回调方法中，添加对应的方法：

```
// 以下系统回调方法仅供参考，具体实现根据您自己的业务需要

// iOS 6 及以前，收到推送
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {

[AnalysysEaManager pushTrack:PUSH_RECEIVE msg:userInfo];
}

// iOS 7 以后 10 以前，收到推送
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {

completionHandler(UIBackgroundFetchResultNewData);

if (application.applicationState == UIApplicationStateActive) {

// App前台 收到推送消息，追踪"App 消息推送"事件
[AnalysysEaManager pushTrack:PUSH_RECEIVE msg:userInfo];

} else if (application.applicationState == UIApplicationStateBackground) {

// App后台 收到推送消息，追踪"App 消息推送"事件
[AnalysysEaManager pushTrack:PUSH_RECEIVE msg:userInfo];

} else {

// 点击通知栏打开消息，记录"App 点击通知"事件
[AnalysysEaManager pushTrack:PUSH_CLICK msg:userInfo];

}
}

// iOS 10 及以后
- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler {

[AnalysysEaManager pushTrack:PUSH_RECEIVE msg:notification.request.content.userInfo];

completionHandler(UNNotificationPresentationOptionBadge
|UNNotificationPresentationOptionAlert);
}
```

#### 5、配置统计推送到达所需的 Notification Service Extension 扩展及 AppGroups （非必须）

配置这两项，主要针对 APP 支持推送，在进程被杀死的情况下统计推送到达率，若 APP 不支持推送功能，可忽略。若 APP 支持推送功能，不配置扩展和 AppGroups 会导致 APP 在进程被杀死的情况下推送到达无法统计，应用在前、后台的情况不受影响。建议您按照如下步骤进行配置。

**配置 Notification Service Extension 扩展 **

APP 进程在被杀死的情况下，iOS 10.0 以后可以通过 Notification Service Extension 扩展服务处理推送到达的统计，10.0 以下版本不支持。APP 客户端需要做如下配置：

* 创建 Notification Service Extension 类型的 target，选择最低支持的版本为 iOS 10.0

* 在系统生成的 NotificationService.m 文件中 didReceiveNotificationRequest 方法里调用 pushTrack 方法追踪推送到达事件

```
- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler {
self.contentHandler = contentHandler;
self.bestAttemptContent = [request.content mutableCopy];

// Modify the notification content here...
self.bestAttemptContent.title = [NSString stringWithFormat:@"%@ [方舟易达]", self.bestAttemptContent.title];

[AnalysysEaManager pushTrack:PUSH_RECEIVE msg:self.bestAttemptContent.userInfo];

self.contentHandler(self.bestAttemptContent);
}
```

**配置 App Groups **

为保证主 APP 进程被杀死的情况下，扩展进程能正常访问主 APP 的某些数据，从而使 SDK 能正常统计推送到达率，APP 客户端需要添加进程间数据共享：

* 选择主 target -》 Capabilities，添加 App Groups，填入分组名 **group.easdk**，若分组名显示为红色，点击下方刷新按钮，直至分组名不再为红色

* 选择 Notification Service Extension target -》 Capabilities，添加 App Groups，勾选分组 **group.easdk**，若分组名显示为红色，点击下方刷新按钮，直至分组名不再为红色

#### 6、成功运行

* 真机调试该项目，如果控制台输出如下日志，代表 SDK 集成成功。

```
********************** [EALog] *********************
[EASDKManager.m:216行] AnalysysEasyTouch 启动成功！
AppKey：ecaaab42502jgdg9870fd0740ce374daa
userId：1BCAF1D0-C8C0-46A8-866F-005832024259
****************************************************
```

### 三、iOS API

* 方舟 SDK 接口请参考 AnalysysAgent 对应文档

* 这里只列举易达 AnalysysEasyTouch 相关接口

#### 获取事件监听对象

**支持的版本**

1.0.0 及以上版本。

**接口说明**

获取事件监听对象，用于监听方舟回调的事件。

**接口定义**

```
+ (id)getObserverListener;
```

**参数说明**

无

**接口返回**

返回事件监听代理对象。

**注意事项**

调用方舟 AnalysysAgent 注册事件监听对象的接口时，传入该接口返回对象。

#### 启动 SDK

**支持的版本**

1.0.0 及以上版本。

**接口说明**

启动易达 SDK。

**接口定义**

```
+ (void)startWithConfig:(AnalysysEaConfig *)config;
```

**参数说明**

* config

* 配置 SDK 启动所需要的 appKey 等信息，AnalysysEaConfig 对象实例

**接口返回**

无

**注意事项**

无

#### 获取 SDK 版本号

**支持的版本**

1.0.0 及以上版本。

**接口说明**

获取 SDK 当前版本号。

**接口定义**

```
+ (NSString *)SDKVersion;
```

**参数说明**

无

**接口返回**

返回 SDK 当前版本号。

**注意事项**

无

#### 注册 APNS 远程推送

**支持的版本**

1.1.0 及以上版本。

**接口说明**

注册 APNS 远程推送，封装了系统注册远程推送的 API。

**接口定义**

```
+ (void)registerForRemoteNotificationWithDelegate:(id)delegate;
```

**参数说明**

* delegate

* 实现系统推送回调方法的类，一般为当前 AppDelegate 类对象本身。

**接口返回**

无

**注意事项**

无

#### 注册推送 deviceToken

**支持的版本**

1.0.9 及以上版本。

**接口说明**

注册 APP 启动后由系统返回的 deviceToken。

**接口定义**

```
+ (void)registerDeviceToken:(NSString *)deviceToken;
```

**参数说明**

* deviceToken

* app 启动后由系统返回的用于推送的 deviceToken。

**接口返回**

无

**注意事项**

注册的 token 需是经过解析后的。

#### 追踪推送消息

**支持的版本**

1.0.0 及以上版本。

**接口说明**

用户在配置了 APNS 通道后，当APP接收到推送消息及点击了推送消息时，在相应的系统回调方法里调用易达 SDK 追踪推送消息的接口来统计推送到达率和点击率。

**接口定义**

```
+ (void)pushTrack:(PushEventType)type msg:(NSDictionary *)msg;
```

**参数说明**

* type

* 消息事件类型枚举。PUSH\_RECEIVE：收到推送，PUSH\_CLICK：点击推送

* msg

* 系统回调的消息内容。

* 应用在前台收到推送，点击回调，msg 传 notification.request.content.userInfo

* 应用在后台收到推送，点击回调，msg 传 response.notification.request.content.userInfo

* 应用进程被杀死的情况下收到推送，推送到达回调，msg 传 request.content.userInfo

**接口返回**

无

**注意事项**

需要在对应的系统回调方法里调用，并根据具体消息事件类型（收到推送/点击推送）传入对应的参数。详细见下方备注。

### 四、备注

#### APNS 推送

易达 SDK 接通了苹果 APNS 推送服务，并支持统计推送到达率、点击率。

* 登录[易达系统](https://ea.analysys.cn:8088/app.html#/Login)，进行 push 配置，选择  并上传 iOS 生产证书、填写证书秘钥。

* 对应接口，详细参考 iOS API

```
+ (void)pushTrack:(PushEventType)type msg:(NSDictionary *)msg;
```

### 五、FAQ

#### 集成动态库后，添加 Notification Service Extension 扩展并在代理方法中调用 SDK 方法，编译报错

* 动态库目前不支持在该扩展中调用三方 SDK，换用静态库集成即可

* 如果必须使用动态库集成，则需删除 Extension

#### 后台统计的推送到达不准或无数据

* 若后台统计不到推送到达，先检查 APP 是否添加了 Notification Service Extension 扩展，且添加了 App Groups 并设置其 Id 为 **group.easdk**

#### 添加 Notification Service Extension 扩展并在相应代理方法中调用 SDK 方法，编译报错

* 若报错提示信息类似如下，可能是添加静态库时没有将其引入到 Notification Service Extension 对应的 target 中，解决办法是重新将静态库拖入工程中并勾选上主 target 和 扩展 target 即可

```
Undefined symbols for architecture arm64:
"_OBJC_CLASS_$_AnalysysEaManager", referenced from:
objc-class-ref in NotificationService.o
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

### 六、技术支持

当出现问题时：

* 请仔细阅读文档，查看是否有遗漏。

* 给我们的技术 support 发邮件：guoyongqing@analysys.com.cn


