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

/// 启动SDK
/// @param config 配置信息
+ (void)startWithConfig:(AnalysysEaConfig *)config;

/// 获取 SDK 版本号
+ (NSString *)SDKVersion;

#pragma mark - 推送

/// 追踪推送消息
/// @param type 推送消息事件类型，到达/点击
/// @param msg 推送消息
+ (void)pushTrack:(PushEventType)type msg:(NSDictionary *)msg;

@end

NS_ASSUME_NONNULL_END
