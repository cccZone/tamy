/// @file   TamyEditor/ProfilerChart.h
/// @brief  a chart showing the runtime traces of a profiled timer
#pragma once

#include <QGraphicsView>
#include <QPen>
#include <QPointF>
#include "core/types.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class QGraphicsScene;
class QGraphicsSimpleTextItem;

///////////////////////////////////////////////////////////////////////////////

class ProfilerChart : public QGraphicsView
{
   Q_OBJECT

private:
   struct PointsListNode
   {
      float                            m_value;
      PointsListNode*                  m_nextNode;
      PointsListNode*                  m_prevNode;

      PointsListNode() : m_value( 0.0f ), m_nextNode( NULL ), m_prevNode( NULL ) {}
   };

public:
   uint                                m_timerId;

   QGraphicsScene*                     m_scene;
   QPen                                m_chartPen;

   const float                         DISPLAYED_DURATION;
   const float                         SAMPLING_INTERVAL;

   float                               m_topValue;

   std::vector< QGraphicsLineItem* >   m_chartLines;
   PointsListNode*                     m_chartPointsHead;
   uint                                m_samplesCount;

   float                               m_sampleValue;
   float                               m_samplingTime;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    * @param timerId          id of the charted timer
    */
   ProfilerChart( QWidget* parentWidget, uint timerId );
   ~ProfilerChart();

   /**
    * Adds a new sample to the chart.
    *
    * @param elapsedTime      time elapsed since the last sample was added
    * @param sampleValue
    *
    * @return                 top displayed value
    */
   float addSample( float elapsedTime, float sampleValue );

};

///////////////////////////////////////////////////////////////////////////////
