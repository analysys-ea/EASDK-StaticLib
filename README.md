# iOS SDK

* iOS 常见问题

* [iOS 客户端 SDK 下载](https://github.com/analysys-ea/EASDK-StaticLib)

---

### 一、iOS SDK 简介

易达系统是易观基于方舟平台一款触达用户的产品，加强了产品的用户体验和提升了产品的转化率，易达的 iOS SDK 是此系统中重要的支撑点，它提供了：

* 提供触达用户的弹窗功能，支持弹窗样式：

* 图片样式，支持点击事件。

* 文本样式，支持标题、正文、两个按钮。

* 图文混合样式，最上面是图片，下面是标题、正文、按钮。

* 订阅事件，用户在后台如果创建了基于用户事件的触发行为（弹窗除外），SDK 就会在每次启动的时候拉取订阅列表，并在有订阅事件产生的时候通知易达系统。

#### iOS 版本支持

支持的 iOS 系统版本为 8.0 及以上版本。

#### 组成

* 动态库文件：

```
EASDK.framework
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

#### 创建项目

* 登录[易达系统](https://ea.analysys.cn:8088/app.html#/Login)，创建项目，项目创建完成后自动生成对应的AppKey用以标识该项目（应用）。

#### 导入 SDK

**选择1：cocoapods 导入动态库**

* 打开 Podfile 文件，添加如下代码

```
pod 'EASDK' // 易达 SDK
```

* 如果需要安装指定版本，则按照以下方式

```
pod 'EASDK', '1.0' // 示例版本号
```
* 特别注意：由于iOS 10以后苹果系统增加的 NSNotification Service Extension 扩展能够用于统计推送到达率，如果在 APP 中添加了该扩展而无法引入第三方的类文件，则需要使用以下“选择2”方式手动下载静态库并导入项目。将静态库及相关头文件添加到项目中的时候，需要同时勾选项目主 target 和 NSNotification Service Extension 扩展target，否则编译会报错。


**选择2：手动下载静态库导入**

* [下载最新静态库 SDK](https://github.com/analysys-ea/EASDK-StaticLib)

* 解压缩后，拷贝 ea-ios-sdk-static 文件夹中的静态库.a文件和对应的.h头文件到项目中

* 选中项目 target ，在 Build Phases 》Link Binary With Libiaries 中检查对应的静态库.a文件是否已经添加，没有则需手动添加进去。

#### 添加头文件

* 如果使用的是动态库，在 AppDelegate.m 中引入以下头文件：

```
#import <EASDK/AnalysysEaManager.h> // 易达 SDK
```
* 如果使用的是静态库，在 AppDelegate.m 中引入以下头文件：

```
#import "AnalysysEaManager.h" // 易达 SDK
```


#### 添加初始化代码

* 请将以下代码添加到 -\(BOOL\)application:\(UIApplication \*\)application didFinishLaunchingWithOptions:\(NSDictionary \*\)launchOptions

```
// 关键！设置事件接受者，这样 EASDK 才能从方舟 SDK 获取事件
[AnalysysAgent setEventReceiver:[AnalysysEaManager getObserverListener]];

/*********** 易达 SDK 初始化 ***********/
AnalysysEaConfig *config = [AnalysysEaConfig defaultConfiguration];
config.appKey= @"易达后台创建项目的 AppKey";
[AnalysysEaManager startWithConfig:config];
```

* 通过方舟 SDK 上报相关用户属性：

```
// 上报pushId（解析后的deviceToken）
[AnalysysAgent setPushProvider:AnalysysPushAPNS pushID:hexToken];
// 上报用户手机号码
[AnalysysAgent profileSet:@"$PHONE" propertyValue:value];
// 上报用户邮箱
[AnalysysAgent profileSet:@"$EMAIL" propertyValue:value];
// 上报用户微信openID
[AnalysysAgent profileSet:@"$WECHATOPENID" propertyValue:value];
```

* 通过方舟 SDK 埋点：

```
// 对应埋点事件
[AnalysysAgent track:@"事件ID" properties:@{事件属性}];
```

#### 成功运行

* 真机调试该项目，如果控制台输出如下日志，代表 SDK 集成成功。

```
********************** [EALog] *********************
[EASDKManager.m:216行] EASDK 启动成功！
AppKey：ecaaab42502jgdg9870fd0740ce374daa
userId：1BCAF1D0-C8C0-46A8-866F-005832024259
*************************************************
```

### 三、iOS API

* 方舟 SDK 接口请参考 AnalysysAgent 对应文档

* 这里只列举易达 EASDK 相关接口

#### 设置事件接收者

**支持的版本**

4.3.5 及以上版本。

**接口说明**

设置事件接收代理对象。当方舟 SDK 每次用户触发事件后，将会发送事件数据到指定代理对象，在代理类中实现的代理方法将会接收并处理对应事件。

**接口定义**

```
+ (void)setEventReceiver:(id)receiver;
```

**参数说明**

* receiver

* 事件接收者，这里设置为下面的获取事件接收代理对象，通过 \[AnalysysEaManager getObserverListener\] 方法获取，否则 SDK 将可能无法正常接收事件。

**接口返回**

无

**注意事项**

此接口为方舟 SDK 接口，通过 AnalysysAgent 调用。

#### 获取事件接收代理

**支持的版本**

1.0及以上版本。

**接口说明**

获取事件接收代理，用户从方舟 SDK 获取用户事件，这里将 EASDKManager 对象作为事件接收者。

**接口定义**

```
+ (id)getObserverListener;
```

**参数说明**

无

**接口返回**

返回事件接收对象。

**注意事项**

无

#### 启动 SDK

**支持的版本**

1.0及以上版本。

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

#### 追踪推送消息

**支持的版本**

1.0及以上版本。

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

* 消息内容。将收到的 NSDictionary 类型的消息内容传入。

**接口返回**

无

**注意事项**

需要在对应的系统回调方法里调用，并根据具体消息事件类型（收到推送/点击推送）传入对应的参数。详细见下方备注。

### 四、备注

#### APNS 推送

易达 SDK 接通了苹果 APNS 推送服务，并支持统计推送到达率、点击率。

* 登录[易达系统](https://ea.analysys.cn:8088/app.html#/Login)，进行 push 配置，选择  并上传 iOS 生产证书、填写证书秘钥。

* 详细图示见 入门指南》项目设置。

* 对应接口，详细参考 iOS API

```
+ (void)pushTrack:(PushEventType)type msg:(NSDictionary *)msg;
```

#### 配置 Notification Service Extension 扩展

APP 进程在被杀死的情况下，iOS 10.0 以后可以通过 Notification Service Extension 扩展服务处理推送到达的统计，10.0 以下版本不支持。APP 客户端需要做如下配置：

* 创建 Notification Service Extension 类型的 target，选择最低支持的版本为 iOS 10.0

* 在系统生成的 NotificationService.m 文件中 didReceiveNotificationRequest 方法里调用 pushTrack 方法追踪推送到达事件

#### 配置 App Groups

为保证主 APP 进程被杀死的情况下，扩展能正常访问主 APP 的某些数据，APP 客户端需要添加进程间数据共享：

* 选择主 target -》 Capabilities，添加 App Groups，填入分组名 group.easdk，若分组名显示为红色，点击下方刷新按钮，直至分组名不再为红色

* 选择 Notification Service Extension target -》 Capabilities，添加 App Groups，勾选分组 group.easdk，若分组名显示为红色，点击下方刷新按钮，直至分组名不再为红色

### 六、FAQ

#### 添加 Notification Service Extension 扩展并在相应代理方法中调用 SDK 方法，编译报错

* 若报错提示信息类似如下，可能是添加静态库时没有将其引入到 Notification Service Extension 对应的 target 中，解决办法是重新将静态库拖入工程中并勾选上主 target 和 扩展 target 即可

```
Undefined symbols for architecture arm64:
"_OBJC_CLASS_$_AnalysysEaManager", referenced from:
objc-class-ref in NotificationService.o
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

### 七、技术支持

当出现问题时：

* 请仔细阅读文档，查看是否有遗漏。

* 给我们的技术 support 发邮件：shadeless99@126.com




