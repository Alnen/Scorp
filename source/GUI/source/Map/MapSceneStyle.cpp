#include "Scorp/GUI/Map/MapSceneStyle.h"

namespace MapSceneStyle
{

// struct AreaStyle
AreaStyle::AreaStyle()
    : fillColor(QColor::fromRgb(0, 0, 0)), borderColor(QColor::fromRgb(0, 0, 0)), borderWidth(1.f), imagePath("")
{
}

AreaStyle::AreaStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width)
    : fillColor(_fill_color), borderColor(_border_color), borderWidth(_border_width), imagePath("")
{
}

// struct MarkerStyle   
MarkerStyle::MarkerStyle()
    : AreaStyle(), radius(5.f)
{
}

MarkerStyle::MarkerStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width, float _radius)
    : AreaStyle(_fill_color, _border_color, _border_width), radius(_radius)
{
}

// struct TransitionStyle
TransitionStyle::TransitionStyle()
    : AreaStyle(), width(5.f), height(10.f)
{
}

TransitionStyle::TransitionStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width, float _width, float _height)
    : AreaStyle(_fill_color, _border_color, _border_width), width(_width), height(_height)
{
}

// struct StateStyle  
StateStyle::StateStyle()
    : AreaStyle(), radius(5.f)
{
}

StateStyle::StateStyle(const QColor& _fill_color, const QColor& _border_color, float _border_width, float _radius)
    : AreaStyle(_fill_color, _border_color, _border_width), radius(_radius)
{
}

// struct TrackStyle
TrackStyle::TrackStyle()
    : color(QColor::fromRgb(0, 0, 0)), lineWidth(1.f), arrowLenght(5.f), arrowWidth(6.f)
{    
}

TrackStyle::TrackStyle(const QColor& _color, float _line_width, float _arrow_lenght, float _arrow_width)
    : color(_color), lineWidth(_line_width), arrowLenght(_arrow_lenght), arrowWidth(_arrow_width)
{
}

// struct LinkStyle
LinkStyle::LinkStyle()
    : trainStyle(QColor::fromRgb(255, 0, 0), QColor::fromRgb(0, 0, 0), 1.f, 5.f),
    accessTokenStyle(QColor::fromRgb(255, 255, 0), QColor::fromRgb(0, 0, 0), 1.f, 5.f),
    exitTransitionStyle(QColor::fromRgb(0, 200, 200), QColor::fromRgb(0, 0, 0), 1.f, 6.f, 18.f),
    enterTransitionStyle(QColor::fromRgb(200, 100, 0), QColor::fromRgb(0, 0, 0), 1.f, 6.f, 18.f),
    semaphoreStyle(QColor::fromRgb(200, 200, 200), QColor::fromRgb(0, 0, 0), 1.f, 10.f),
    interStateStyle(QColor::fromRgb(200, 200, 0), QColor::fromRgb(0, 0, 0), 1.f, 10.f),
    trackStyle(QColor::fromRgb(0, 0, 0), 1.f, 5.f, 6.f),
    linkView(LinkViewType::Flexible)
{
}

// struct MapSceneStyle
MapSceneStyle::MapSceneStyle()
    : LinkStyle(), stationStyle(QColor::fromRgb(0, 200, 0), QColor::fromRgb(0, 0, 0), 2.f, 10.f)
{
}

LinkStyle MapSceneStyle::getLinkStyle() const
{
    LinkStyle link_style;
    link_style.trainStyle = this->trainStyle;
    link_style.accessTokenStyle = this->accessTokenStyle;
    link_style.exitTransitionStyle = this->exitTransitionStyle;
    link_style.enterTransitionStyle = this->enterTransitionStyle;
    link_style.semaphoreStyle = this->semaphoreStyle;
    link_style.interStateStyle = this->interStateStyle;
    link_style.trackStyle = this->trackStyle;
    link_style.linkView = this->linkView;
    return link_style;
}

}
