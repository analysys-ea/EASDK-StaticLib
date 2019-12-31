//
//  AnalysysEaConfig.h
//  EASDK
//
//  Created by 郭永青 on 2019/10/11.
//  Copyright © 2019 Analysys Technology Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// SDK 运行环境
typedef NS_ENUM(NSUInteger, EnvType) {
    ENV_DISTRIBUTION, // 线上环境
    ENV_TEST,         // 测试环境
    ENV_DEV,          // 开发环境
};

/// 处理推送消息的类型，推送触达或者点击了推送消息
typedef NS_ENUM(NSUInteger, PushEventType) {
    PUSH_RECEIVE, // 收到推送消息
    PUSH_CLICK    // 点击推送消息
};

/// 弹窗活动数据拉取方式
typedef NS_ENUM(NSUInteger, DataFetchType) {
    DATA_FETCH_EACH_TIME, // 每次启动拉取一次数据
    DATA_FETCH_EACH_DAY,  // 每天拉取一次数据
};

/// SDK 初始化配置类
@interface AnalysysEaConfig : NSObject

/// 获取 AnalysysEaConfig 对象唯一实例，建议使用此单例方法
+ (instancetype)defaultConfig;

/// 初始化时传入的 appKey ，由创建项目时自动生成
@property (nonatomic, copy) NSString *appKey;
/// SDK 运行环境，默认为线上环境
@property (nonatomic, assign) EnvType envType;
/// 弹窗活动数据拉取方式，默认为每次启动 APP 都会请求获取一次
@property (nonatomic, assign) DataFetchType dataFetchType;


@end

NS_ASSUME_NONNULL_END
