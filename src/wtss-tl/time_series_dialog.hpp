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
  with WTSS.TL. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-tl/time_series_dialog.hpp

  \brief Time series query dialog for Web Time Series Services plugin.

  \author Matheus Cavassan Zaglia
 */

#ifndef __WTSS_TL_TIME_SERIES_DIALOG_HPP__
#define __WTSS_TL_TIME_SERIES_DIALOG_HPP__

//wtss.cxx
#include <wtss-cxx/data_types.hpp>

//STL
#include <memory>
#include <string>

//QT
#include <QDialog>
#include <QObject>
#include <QtGui>
#include <QListWidgetItem>
#include "qcustomplot.h"

namespace Ui { class time_series_dialog_form; }

namespace wtss_tl{
  class time_series_dialog : public QDialog
  {
    Q_OBJECT

    public:

      time_series_dialog(wtss_cxx::timeseries_query_t query, QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~time_series_dialog();

    protected slots:

//      void scrollBarChanged(int value);

//      void xAxisChanged(QCPRange range);

      void onYAxisRangeChanged(QCPRange range);
      void onXAxisRangeChanged(QCPRange range);
  protected:

      wtss_cxx::timeseries_query_t query;
      wtss_cxx::timeseries_query_result_t result;
      std::string server_uri;

      void loadSettings();

      void doQuery();

      void plotResult();

  private:

      std::auto_ptr<Ui::time_series_dialog_form> m_ui;
      QColor randomColor();
      double lowerBound;
      double upperBound;
  };
}

#endif //__WTSS_TL_TIME_SERIES_DIALOG_HPP__