//
//  BezierLayer.h
//  DrawBezier
//
//  Created by 邱婧 on 14-2-26.
//
//

#ifndef __DrawBezier__BezierLayer__
#define __DrawBezier__BezierLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum DraggingSprite
{
    DraggingSpriteNone,
    DraggingSpriteControlPoint1,
    DraggingSpriteControlPoint2,
    DraggingSpriteEndPosition,
    DraggingSpriteFish
};

class BezierLayer:public CCLayer
{
public:
    BezierLayer();
    ~BezierLayer();
    
    virtual bool init();
    CREATE_FUNC(BezierLayer);
    
    static CCScene *scene();
    
    DraggingSprite dragging;
    bool bezierLTR;

public:
    CCPoint controlPoint1;
    CCPoint controlPoint2;
    CCPoint endPosition;
    CCPoint fishPosition;
private:
    void initBezierMovement();
    void performBezierMovement();
    void stopBezierMovement();
    void speedUpMovement();
    void speedDownMovement();
    void resetNodesPosition();
    void addLabel(CCPoint position, const char* labelText, int labelTag);
    CCString *formatPointAndName(CCPoint point, const char* name);
    void bezierActionFinished(CCNode *node);
    void valueChanged(CCObject *sender, CCControlEvent controlEvent);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void draw();
};

#endif /* defined(__DrawBezier__BezierLayer__) */
