//
//  AnalysysEaManager.h
//  EASDK
//
//  Created by 郭永青 on 2019/10/9.
//  Copyright © 2019 Analysys Technology Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AnalysysEaConfig.h"

NS_ASSUME_NONNULL_BEGIN

@interface AnalysysEaManager : NSObject

#pragma mark - 基础功能

/// 获取事件监听对象，用于实时捕获系统和用户事件
+ (id)getObserverListener;

/// 启动SDK
/// @param config 配置信息
+ (void)startWithConfig:(AnalysysEaConfig *)config;

/// 获取 SDK 版本号
+ (NSString *)SDKVersion;

#pragma mark - 推送

/// 注册 APNS 远程推送
/// @param delegate 实现系统推送通知回调方法的代理类对象，通常为 AppDelegate
+ (void)registerForRemoteNotificationWithDelegate:(id)delegate;

/// 注册远程推送获取的 deviceToken
/// @param deviceToken APNS 远程推送由系统返回的 deviceToken
+ (void)registerDeviceToken:(NSString *)deviceToken;

/// 追踪推送消息
/// @param type 推送消息事件类型，到达/点击
/// @param msg 推送消息
+ (void)pushTrack:(PushEventType)type msg:(NSDictionary *)msg;

#pragma mark - 别名

/// 显示别名标签
+ (void)showAliasTag;

/// 隐藏别名标签
+ (void)hideAliasTag;

@end

NS_ASSUME_NONNULL_END
