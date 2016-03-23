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
  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-tl/server_config_dialog.cpp

  \brief Configuration dialog for Web Time Series Services plugin.

  \author Matheus Cavassan Zaglia
 */


// wtss.cxx
#include <wtss-cxx/wtss.hpp>

// wtss.tl
#include "server_config_dialog.hpp"
#include "ui_server_config_dialog_form.h"
#include "services_manager.hpp"

// TerraLib
#include <terralib/qt/widgets/utils/ScopedCursor.h>

// QT
#include <QInputDialog>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string/join.hpp>

wtss_tl::server_config_dialog::server_config_dialog(QWidget *parent, Qt::WindowFlags f):
QDialog(parent, f),
dirty(false),
m_ui(new Ui::server_config_dialog_form)
{
  m_ui->setupUi(this);
  this->setWindowTitle(tr("Web Time Series Services - Settings"));
  this->setFixedSize(400,500);
  m_ui->btnAddServer->setIcon(QIcon::fromTheme("list-add"));
  m_ui->btnRemoveServer->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->btnRefreshServer->setIcon(QIcon::fromTheme("view-refresh"));

  connect(m_ui->btnAddServer, SIGNAL(clicked()),this,SLOT(onServerAddButtonClicked()));
  connect(m_ui->btnRemoveServer, SIGNAL(clicked()),this,SLOT(onServerRemoveButtonClicked()));
  connect(m_ui->cboServices,SIGNAL(currentIndexChanged(QString)),this, SLOT(onComboServerSelected()));
  connect(m_ui->cboCoverages,SIGNAL(currentIndexChanged(QString)),this, SLOT(onComboCoverageChanged(QString)));
  connect(m_ui->listAttributes, SIGNAL(itemChanged(QListWidgetItem*)),this, SLOT(onListAtrributesChecked(QListWidgetItem*)));
  connect(m_ui->btnClose,SIGNAL(clicked()),this, SLOT(onCloseButtonClicked()));
  connect(m_ui->btnRefreshServer,SIGNAL(clicked()), this, SLOT(onServerRefreshButtonClicked()));

  j_config = wtss_tl::services_manager::getInstance().loadConfig().object();
  if(j_config.keys().size() > 0)
      m_ui->cboServices->addItems(j_config.keys());

}

wtss_tl::server_config_dialog::~server_config_dialog()
{

}

void wtss_tl::server_config_dialog::onServerAddButtonClicked()
{
  QInputDialog* inputDialog = new QInputDialog();
  inputDialog->setOptions(QInputDialog::NoButtons);
  bool ok;
  QString uri = inputDialog->getText(NULL,"Add Server","Server URI:",QLineEdit::Normal,"", &ok);
  if(!uri.isEmpty())
  {
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    te::qt::widgets::ScopedCursor c(Qt::WaitCursor);
    wtss_tl::services_manager::getInstance().addServer(uri);
    j_config = wtss_tl::services_manager::getInstance().loadConfig().object();
    m_ui->cboServices->addItem(uri);
  }
}

void wtss_tl::server_config_dialog::onServerRemoveButtonClicked()
{
  if(m_ui->cboServices->count() > 0)
  {
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Remove server", "Remove '"+
                                    m_ui->cboServices->currentText() +"' server?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes)
      {
        wtss_tl::services_manager::getInstance().removeServer(m_ui->cboServices->currentText());
        j_config = wtss_tl::services_manager::getInstance().loadConfig().object();
        m_ui->cboServices->removeItem(m_ui->cboServices->currentIndex());
        m_ui->cboCoverages->clear();
        m_ui->listAttributes->clear();
      }
    }
}

void wtss_tl::server_config_dialog::onServerRefreshButtonClicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  wtss_tl::services_manager::getInstance().addServer(m_ui->cboServices->currentText());
  j_config = wtss_tl::services_manager::getInstance().loadConfig().object();
  QApplication::restoreOverrideCursor();
}

void wtss_tl::server_config_dialog::onComboServerSelected()
{
  if(m_ui->cboServices->currentText().size() > 0)
  {
    m_ui->cboCoverages->clear();
    m_ui->listAttributes->clear();
    server_uri = m_ui->cboServices->currentText();
    QJsonObject j_server = j_config.find(server_uri).value().toObject();
    QJsonObject j_coverages = j_server["coverages"].toObject();
    QJsonObject::iterator it;

    for(it = j_coverages.begin(); it != j_coverages.end(); it++)
    {
      QString cv = it.key();
      QJsonObject j_coverage = it.value().toObject();
      m_ui->cboCoverages->addItem(cv);
      if(j_coverage["active"].toBool())
      {
        m_ui->cboCoverages->setCurrentText(cv);
        coverage = cv;
        QJsonObject::iterator it_at;
        QJsonObject j_attributes = j_coverage.find("attributes").value().toObject();
        for(it_at = j_attributes.begin(); it_at != j_attributes.end(); it_at++)
        {
          QString attribute = it_at.key();
          bool active = it_at.value().toBool();
          QListWidgetItem* a = new QListWidgetItem(attribute, m_ui->listAttributes);
          a->setFlags(a->flags() | Qt::ItemIsUserCheckable);
          if(active)
            a->setCheckState(Qt::Checked);
          else
            a->setCheckState(Qt::Unchecked);
        }
      }
    }
    dirty = true;
  }
}

void wtss_tl::server_config_dialog::onComboCoverageChanged(QString coverage)
{
  if(dirty)
  {
    if(m_ui->cboServices->currentText().size() > 0)
    {
      m_ui->listAttributes->clear();
      j_config = wtss_tl::services_manager::getInstance().loadConfig().object();

      QJsonObject j_server = j_config.find(server_uri).value().toObject();
      QJsonObject j_coverages = j_server["coverages"].toObject();
      QJsonObject::iterator it;

      for(it = j_coverages.begin(); it != j_coverages.end(); it++)
      {
        QJsonObject j_coverage = it.value().toObject();
        if(j_coverage["active"].toBool())
          wtss_tl::services_manager::getInstance().changeStatusCoverage(server_uri, it.key());
      }

      wtss_tl::services_manager::getInstance().changeStatusCoverage(server_uri, coverage);
      QJsonObject j_coverage = j_coverages.find(coverage).value().toObject();
      QJsonObject::iterator it_at;
      QJsonObject j_attributes = j_coverage.find("attributes").value().toObject();

      for(it_at = j_attributes.begin(); it_at != j_attributes.end(); it_at++)
      {
        QString attribute = it_at.key();
        bool active = it_at.value().toBool();
        QListWidgetItem* a = new QListWidgetItem(attribute, m_ui->listAttributes);
        a->setFlags(a->flags() | Qt::ItemIsUserCheckable);
        if(active)
          a->setCheckState(Qt::Checked);
        else
          a->setCheckState(Qt::Unchecked);
      }
    }
  }
}

void wtss_tl::server_config_dialog::onListAtrributesChecked(QListWidgetItem *item)
{
  wtss_tl::services_manager::getInstance().changeStatusAttribute(m_ui->cboServices->currentText(), m_ui->cboCoverages->currentText(), item->text());
}

void wtss_tl::server_config_dialog::onCloseButtonClicked()
{
  this->close();
}

