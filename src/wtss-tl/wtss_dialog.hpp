/*
  Copyright (C) 2016 National Institute For Space Research (INPE) - Brazil.

  This file is part of the WTSS.TL.

  WTSS.TL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  WTSS.TL is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with TerraLib Web Services. See COPYING. If not, see
  <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-tl/wtss_dialog.hpp

  \brief Configuration dialog for Web Time Series Services plugin.

  \author Matheus Cavassan Zaglia
 */

#ifndef __WTSS_TL_WTSS_DIALOG_HPP__
#define __WTSS_TL_WTSS_DIALOG_HPP__

// STL
#include <memory>
#include <string>

// QT
#include <QColor>
#include <QDialog>
#include <QListWidgetItem>
#include <QObject>
#include <QTreeWidget>
#include <QtGui>
#include <QGridLayout>

// wtss.cxx
#include <wtss-cxx/data_types.hpp>

// TerraLib
#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/canvas/Canvas.h>
#include <terralib/common.h>
#include <terralib/datatype/Date.h>

#include <terralib/qt/widgets/charts/ChartDisplay.h>
#include <terralib/qt/widgets/charts/ChartDisplayWidget.h>
#include <terralib/qt/widgets/charts/ChartStyle.h>
#include <terralib/qt/widgets/charts/TimeSeriesChart.h>
#include <terralib/st/core/timeseries/TimeSeries.h>
#include <terralib/se/Mark.h>


namespace Ui
{
  class wtss_dialog_form;
}

namespace wtss
{
  namespace tl
  {
    enum tree_item_type
    {
      server,
      coverage,
      attribute
    };
    class wtss_dialog : public QDialog
    {
      Q_OBJECT

     public:
      wtss_dialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~wtss_dialog();

      void set_map_display(te::qt::widgets::MapDisplay* mapDisplay);

      void do_timeseries_query(wtss::cxx::timeseries_query_t query);

      void hide_graph(bool check);

      void onPointPicked(QPointF& coord);

      void closeEvent(QCloseEvent* e);

     public slots:

      void onPointPickerToggled(bool checked);

      void updateZoom();

     protected slots:

      void onServerAddButtonClicked();

      void onServerRemoveButtonClicked();

      void onServerRefreshButtonClicked();

      void onItemChecked(QTreeWidgetItem* item);

      void onHideButtonClicked();

      void onHideCoordSelectedsClicked();

      void onHelpButtonClicked();

      void onCloseButtonClicked();

      void onExportGraphClicked();

      void onImportGraphClicked();

      void onQueryButtonClicked();

      void onAddCoordToList(QListWidgetItem* coordSelected);

      void onGetPointCoordenate(QPointF &coord);

     signals:

      void pointPicked(QPointF& coord);

      void close();

     private:

      void load_settings();

      void define_display();

      void define_marker();

      void add_server(QString server);

      void add_coverage(QTreeWidgetItem* server);

      void add_atributes(QTreeWidgetItem* coverageItem, QJsonObject j_coverage);

      void add_result_to_plot();

      bool validate_query();

      void plot_time_series();

      void addMarker(double x, double y);

      void clearCanvas();

      QColor random_color();

      std::vector<te::st::TimeSeries*> get_time_series();

     private:

      std::auto_ptr<Ui::wtss_dialog_form> m_ui;

      bool dirty;

      bool m_checkServer;

      bool m_checkCoverage;

      bool m_checkAttribute;

      double m_lowerBound;

      double m_upperBound;

      std::string m_lastQueriedServer;

      QJsonObject j_config;

      te::qt::widgets::MapDisplay* m_mapDisplay;

      te::qt::widgets::ChartDisplay* m_chartDisplay;

      te::qt::widgets::ChartStyle* m_chartStyle;

      te::qt::widgets::TimeSeriesChart* m_timeSeriesChart;

      std::vector<te::st::TimeSeries*> m_timeSeriesVec;

      wtss::cxx::timeseries_query_result_t m_result;

      te::color::RGBAColor** m_rgbaMarker;

      te::se::Mark* m_marker;

    };
  }  // end namespace tl
}  // end namespace wtss

#endif  //__WTSS_TL_WTSS_DIALOG_HPP__
