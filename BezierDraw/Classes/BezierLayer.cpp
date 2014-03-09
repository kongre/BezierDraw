//
//  BezierLayer.cpp
//  DrawBezier
//
//  Created by 邱婧 on 14-2-26.
//
//

#include "BezierLayer.h"

#define kControlPoint1Title  "Control Point 1"
#define kControlPoint2Title "Control Point 2"
#define kEndPointTitle "End Point"
#define kCatSpriteTitle "fish"
#define kLabelTitleMake(str) CCString::create(str)

#define kOneOrderTitle "一阶曲线"
#define kTwoOrderTitle "二阶曲线"

enum kActionTag{
    kTagFishSprite = 1001,
    kTagFishSpriteLabel,
    kTagControlPoint1Sprite,
    kTagControlPoint1Label,
    kTagControlPoint2Sprite,
    kTagControlPoint2Label,
    kTagEndPositionSprite,
    kTagEndPositionLabel,
    kTagSwitchSprite,
    kTagSwitchLabel
};

BezierLayer::BezierLayer()
{
    
}

BezierLayer::~BezierLayer()
{
    
}

bool BezierLayer::init()
{
    bool pRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        CCSize winSize =CCDirector::sharedDirector()->getWinSize();
       
        this -> addLabel(ccp(200, winSize.height*0.8), kOneOrderTitle, kTagSwitchLabel);
        
        CCControlSwitch *changeOrderSwitch = CCControlSwitch::create(CCSprite::create("bg.png"), CCSprite::create("on.png"), CCSprite::create("off.png"), CCSprite::create("thumb.png"));
        changeOrderSwitch -> setPosition(ccp(200, winSize.height*0.9));
        this -> addChild(changeOrderSwitch, 1,kTagSwitchSprite);
        changeOrderSwitch -> addTargetWithActionForControlEvents(this, cccontrol_selector(BezierLayer::valueChanged), CCControlEventValueChanged);
        changeOrderSwitch -> setSelected(true);
        
        CCControlButton *tButton = CCControlButton::create("开始运动", "", 30);
        tButton -> setPosition(ccp(350, winSize.height*0.9));
        tButton -> addTargetWithActionForControlEvents(this, cccontrol_selector(BezierLayer::performBezierMovement), CCControlEventTouchUpInside);
        this -> addChild(tButton,1);
        
        tButton = CCControlButton::create("停止运动", "", 30);
        tButton -> setPosition(ccp(500, winSize.height*0.9));
        tButton -> addTargetWithActionForControlEvents(this, cccontrol_selector(BezierLayer::stopBezierMovement), CCControlEventTouchUpInside);
        this -> addChild(tButton,1);
        
        tButton = CCControlButton::create("加速运动", "", 30);
        tButton -> setPosition(ccp(350, winSize.height*0.8));
        tButton -> addTargetWithActionForControlEvents(this, cccontrol_selector(BezierLayer::speedUpMovement), CCControlEventTouchUpInside);
        this -> addChild(tButton,1);
        
        tButton = CCControlButton::create("减速运动", "", 30);
        tButton -> setPosition(ccp(500, winSize.height*0.8));
        tButton -> addTargetWithActionForControlEvents(this, cccontrol_selector(BezierLayer::speedDownMovement), CCControlEventTouchUpInside);
        this -> addChild(tButton,1);
        
        tButton = CCControlButton::create("重置位置", "", 30);
        tButton -> setPosition(ccp(200, winSize.height*0.7));
        tButton -> addTargetWithActionForControlEvents(this, cccontrol_selector(BezierLayer::resetNodesPosition), CCControlEventTouchUpInside);
        this -> addChild(tButton,1);
        
        dragging = DraggingSpriteNone;
        bezierLTR = true;
        initBezierMovement();
        this -> setTouchMode(kCCTouchesOneByOne);
        this -> setTouchEnabled(true);
        
        pRet = true;
    } while (0);
    return pRet;
}

void BezierLayer::valueChanged(cocos2d::CCObject *sender, CCControlEvent controlEvent)
{
    CCControlSwitch *pSwitch = (CCControlSwitch*)sender;
    CCLabelTTF *switchLabel = (CCLabelTTF*)this -> getChildByTag(kTagSwitchLabel);
    if (pSwitch -> isOn()) {
        switchLabel -> setString(kOneOrderTitle);
    }else{
        switchLabel -> setString(kTwoOrderTitle);
    }
}

CCScene* BezierLayer::scene()
{
    CCScene *pScene = NULL;
    do {
        CC_BREAK_IF(!CCScene::create());
        pScene = CCScene::create();
        CCLayer *pLayer = BezierLayer::create();
        CC_BREAK_IF(!pLayer);
        pScene->addChild(pLayer);
    } while (0);
    return pScene;
}


void BezierLayer::initBezierMovement()
{
    controlPoint1 = ccp(30, 100);
    controlPoint2 = ccp(150, 300);
    endPosition = ccp(300, 100);
    fishPosition = controlPoint1;
    CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("fishes.plist", "fishes.png");
    CCSprite *fishSprite = CCSprite::createWithSpriteFrameName("Fish_Lv05_Move_001.png");
    this -> addChild(fishSprite, 3, kTagFishSprite);
    
    fishSprite -> setPosition(fishPosition);
    fishSprite -> setAnchorPoint(ccp(0, 0));
    
    CCAnimation *catSpriteAnimation = CCAnimation::create();
    for (int i = 1; i<10; i++) {
        catSpriteAnimation -> addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fish_Lv05_Move_00%d.png",i)->getCString()));
    }
    
    catSpriteAnimation -> setDelayPerUnit(0.2);
    CCAnimate *catAnimate = CCAnimate::create(catSpriteAnimation);
    fishSprite -> runAction(CCRepeatForever::create(catAnimate));
   
    
    CCSprite *controlPoint1Sprite = CCSprite::create("circle_green.png");
    controlPoint1Sprite -> setPosition(controlPoint1);
    this -> addChild(controlPoint1Sprite, 2, kTagControlPoint1Sprite);
    
    CCSprite *controlPoint2Sprite = CCSprite::create("circle_red.png");
    controlPoint2Sprite -> setPosition(controlPoint2);
    this -> addChild(controlPoint2Sprite, 2, kTagControlPoint2Sprite);
    
    CCSprite *endPointSprite = CCSprite::create("circle_blue.png");
    endPointSprite -> setPosition(endPosition);
    this -> addChild(endPointSprite, 2, kTagEndPositionSprite);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this -> addLabel(ccp(winSize.width*0.8, winSize.height*0.95), (this->formatPointAndName(controlPoint1, kCatSpriteTitle)->getCString()), kTagFishSpriteLabel);
    this -> addLabel(ccp(winSize.width*0.8, winSize.height*0.9), (this->formatPointAndName(controlPoint1, kControlPoint1Title)->getCString()), kTagControlPoint1Label);
    this -> addLabel(ccp(winSize.width*0.8, winSize.height*0.85), (this->formatPointAndName(controlPoint2, kControlPoint2Title)->getCString()), kTagControlPoint2Label);
    this -> addLabel(ccp(winSize.width*0.8, winSize.height*0.8), (this->formatPointAndName(endPosition, kEndPointTitle)->getCString()), kTagEndPositionLabel);
    
    
}

void BezierLayer::addLabel(cocos2d::CCPoint position, const char *labelText, int labelTag)
{
    CCLabelTTF *label = CCLabelTTF::create(labelText, "Thonburi", 25);
    label -> setColor(ccc3(255, 255, 255));
    label -> setPosition(position);
    this -> addChild(label, 1, labelTag);
}

void BezierLayer::performBezierMovement()
{
    CCSprite *catSprite = (CCSprite*)this->getChildByTag(kTagFishSprite);
    CCActionInterval *bezierAction = NULL;
    CCCallFuncN *actionDone = CCCallFuncN::create(this, callfuncN_selector(BezierLayer::bezierActionFinished));
    
    ccBezierConfig config;
    CCControlSwitch *pSwitch = (CCControlSwitch*)this -> getChildByTag(kTagSwitchSprite);
    if (pSwitch->isOn()) {
        if (bezierLTR) {
            catSprite->setPosition(controlPoint1);
            config.controlPoint_1 = controlPoint1;
            config.controlPoint_2 = controlPoint2;
            config.endPosition = endPosition;
        }else{
            catSprite->setPosition(endPosition);
            config.controlPoint_1 = endPosition;
            config.controlPoint_2 = controlPoint2;
            config.endPosition = controlPoint1;
        }
    }else{
        if (bezierLTR) {
            catSprite->setPosition(fishPosition);
            config.controlPoint_1 = controlPoint1;
            config.controlPoint_2 = controlPoint2;
            config.endPosition = endPosition;
        }else{
            catSprite->setPosition(endPosition);
            config.controlPoint_1 = controlPoint2;
            config.controlPoint_2 = controlPoint1;
            config.endPosition = fishPosition;
        }
    }
    
    bezierAction = CCBezierTo::create(5.0f, config);
    CCActionInterval *sequenceAction = CCSequence::createWithTwoActions(bezierAction, actionDone);
    CCSpeed *speedAction = CCSpeed::create(sequenceAction, 1.0);
    speedAction->setTag(1001);
    catSprite -> runAction(speedAction);
}

void BezierLayer::stopBezierMovement()
{
    CCSprite *catSprite = (CCSprite*)this->getChildByTag(kTagFishSprite);
    catSprite -> stopAllActions();
}

void BezierLayer::speedUpMovement()
{
    CCSprite *catSprite = (CCSprite*)this->getChildByTag(kTagFishSprite);
    CCSpeed *speed =(CCSpeed*)catSprite->getActionByTag(1001);
    float currentSpeed = speed->getSpeed();
    speed -> setSpeed(currentSpeed+0.05);
    
}

void BezierLayer::speedDownMovement()
{
    CCSprite *catSprite = (CCSprite*)this->getChildByTag(kTagFishSprite);
    CCSpeed *speed =(CCSpeed*)catSprite->getActionByTag(1001);
    float currentSpeed = speed->getSpeed();
    currentSpeed = MAX(0, currentSpeed-0.1);
    speed -> setSpeed(currentSpeed);
}

void BezierLayer::resetNodesPosition()
{
    controlPoint1 = ccp(30, 100);
    controlPoint2 = ccp(150, 300);
    endPosition = ccp(300, 100);
    fishPosition = controlPoint1;
    
    CCSprite *tSprite = (CCSprite*)this -> getChildByTag(kTagFishSprite);
    tSprite -> setPosition(fishPosition);
    
    tSprite = (CCSprite*)this -> getChildByTag(kTagControlPoint1Sprite);
    tSprite -> setPosition(controlPoint1);
    
    tSprite = (CCSprite*)this -> getChildByTag(kTagControlPoint2Sprite);
    tSprite -> setPosition(controlPoint2);
    
    tSprite = (CCSprite*)this -> getChildByTag(kTagEndPositionSprite);
    tSprite -> setPosition(endPosition);
}

void BezierLayer::draw()
{
    ccDrawColor4B(255, 0, 255, 0);
    glLineWidth(3);
    CCControlSwitch *pSwitch = (CCControlSwitch*)this -> getChildByTag(kTagSwitchSprite);
    if (pSwitch->isOn()) {
        ccDrawQuadBezier(controlPoint1, controlPoint2, endPosition, 50);
    }else{
        ccDrawCubicBezier(fishPosition, controlPoint1, controlPoint2, endPosition, 100);
    }
}


void BezierLayer::bezierActionFinished(cocos2d::CCNode *node)
{
    bezierLTR = !bezierLTR;
    this -> performBezierMovement();
}

CCString* BezierLayer::formatPointAndName(cocos2d::CCPoint point, const char* name)
{
    return CCString::createWithFormat("%s : %.2f,%.2f",name,point.x,point.y);
}

bool BezierLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool canBegin = false;
    CCPoint touchPoint = pTouch->getLocationInView();
    touchPoint = CCDirector::sharedDirector() -> convertToGL(touchPoint);
    CCSprite *control1Sprite = (CCSprite*)this->getChildByTag(kTagControlPoint1Sprite);
    CCRect control1Rect = control1Sprite ->boundingBox();
    if (control1Rect.containsPoint(touchPoint)) {
        dragging = DraggingSpriteControlPoint1;
        canBegin = true;
    }
    
    CCSprite *control2Sprite = (CCSprite*)this->getChildByTag(kTagControlPoint2Sprite);
    CCRect control2Rect = control2Sprite ->boundingBox();
    if (control2Rect.containsPoint(touchPoint)) {
        dragging = DraggingSpriteControlPoint2;
        canBegin = true;
    }
    
    CCSprite *endPointSprite = (CCSprite*)this->getChildByTag(kTagEndPositionSprite);
    CCRect endPointRect = endPointSprite ->boundingBox();
    if (endPointRect.containsPoint(touchPoint)) {
        dragging = DraggingSpriteEndPosition;
        canBegin = true;
    }
    
    CCControlSwitch *switchSprite = (CCControlSwitch*)this->getChildByTag(kTagSwitchSprite);
    if (!switchSprite->isOn()) {
        CCSprite *catSprite = (CCSprite*)this ->getChildByTag(kTagFishSprite);
        CCRect catRect = catSprite ->boundingBox();
        if (catRect.containsPoint(touchPoint)) {
            dragging = DraggingSpriteFish;
            canBegin = true;
        }
    }
    return canBegin;
}

void BezierLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (dragging == DraggingSpriteNone) {
        return;
    }
    CCPoint touchLocation = pTouch -> getLocationInView();
    touchLocation = CCDirector::sharedDirector() -> convertToGL(touchLocation);
    CCPoint lastLocation = pTouch -> getPreviousLocationInView();
    lastLocation = CCDirector::sharedDirector() -> convertToGL(lastLocation);
    CCSprite *moveSprite;
    CCLabelTTF *changeLabel;
    const char* labelTilte = NULL;
    switch (dragging) {
        case DraggingSpriteControlPoint1:
        {
            moveSprite = (CCSprite*)this ->getChildByTag(kTagControlPoint1Sprite);
            changeLabel = (CCLabelTTF*)this -> getChildByTag(kTagControlPoint1Label);
            labelTilte = kControlPoint1Title;
            controlPoint1 = lastLocation;
            break;
        }
        case DraggingSpriteControlPoint2:
        {
            moveSprite = (CCSprite*)this ->getChildByTag(kTagControlPoint2Sprite);
            changeLabel = (CCLabelTTF*)this -> getChildByTag(kTagControlPoint2Label);
            labelTilte = kControlPoint2Title;
            controlPoint2 = lastLocation;
            break;
        }
        case DraggingSpriteEndPosition:
        {
            moveSprite = (CCSprite*)this ->getChildByTag(kTagEndPositionSprite);
            changeLabel = (CCLabelTTF*)this -> getChildByTag(kTagEndPositionLabel);
            labelTilte = kEndPointTitle;
            endPosition = lastLocation;
            break;
        }
        case DraggingSpriteFish:
        {
            moveSprite = (CCSprite*)this ->getChildByTag(kTagFishSprite);
            labelTilte = kCatSpriteTitle;
            changeLabel = (CCLabelTTF*)this -> getChildByTag(kTagFishSpriteLabel);
            fishPosition = lastLocation;
        }
        default:
            break;
    }
    moveSprite -> setPosition(lastLocation);
    changeLabel->setString(this -> formatPointAndName(lastLocation, labelTilte) ->getCString());
}

void BezierLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    dragging = DraggingSpriteNone;
}
