#ifndef MAP_SCENE_STYLE_H
#define MAP_SCENE_STYLE_H

#include <QString>
#include <QColor>

namespace MapSceneStyle
{

enum class LinkViewType { Detailed, Generic, Flexible };

struct AreaStyle
{
    QColor fillColor;
    QColor borderColor;
    float borderWidth;
    QString imagePath;
    
    AreaStyle();
    AreaStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width);
};

struct MarkerStyle : public AreaStyle
{
    float radius;
    
    MarkerStyle();
    MarkerStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width, float _radius);
};

struct TransitionStyle : public AreaStyle
{
    float width;
    float height;
    
    TransitionStyle();
    TransitionStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width, float _width, float _height);
};

struct StateStyle : public AreaStyle
{
    float radius;
    
    StateStyle();
    StateStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width, float _radius);
};

struct TrackStyle
{
    QColor color;
    float lineWidth;
    float arrowLenght;
    float arrowWidth;
    
    TrackStyle();
    TrackStyle(const QColor& _color, float _line_width, float _arrow_lenght, float _arrow_width);
};

struct LinkStyle
{
    MarkerStyle trainStyle;
    MarkerStyle accessTokenStyle;
    TransitionStyle exitTransitionStyle;
    TransitionStyle enterTransitionStyle;
    StateStyle semaphoreStyle;
    StateStyle interStateStyle;
    TrackStyle trackStyle;
    LinkViewType linkView;
    
    LinkStyle();
};

struct MapSceneStyle : public LinkStyle
{
    StateStyle stationStyle;
    
    MapSceneStyle();
    LinkStyle getLinkStyle() const;
};

}

#endif // MAP_SCENE_STYLE_H