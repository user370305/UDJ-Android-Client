/**
 * Copyright 2011 Kurtis L. Nusbaum
 * 
 * This file is part of UDJ.
 * 
 * UDJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * UDJ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with UDJ.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "PlaylistView.hpp"
#include "MusicLibrary.hpp"
#include <QHeaderView>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include "PlaylistDelegate.hpp"
#ifdef UDJ_DEBUG_BUILD
  #include <QSqlError>
  #include <iostream>
#endif

namespace UDJ{

PlaylistView::PlaylistView(MusicLibrary* musicLibrary, QWidget* parent):
  QTableView(parent),
  musicLibrary(musicLibrary),
  database(musicLibrary->database())
{
  playlistModel = new QSqlRelationalTableModel(this, database);
  playlistModel->setTable("main_playlist_view");
  playlistModel->select();
  playlistModel->setHeaderData(0, Qt::Horizontal, "playlist id");
  playlistModel->setHeaderData(1, Qt::Horizontal, "library id");
  playlistModel->setHeaderData(2, Qt::Horizontal, "Song");
  playlistModel->setHeaderData(3, Qt::Horizontal, "Artist");
  playlistModel->setHeaderData(4, Qt::Horizontal, "Album");
  playlistModel->setHeaderData(5, Qt::Horizontal, "Filepath");
  playlistModel->setHeaderData(6, Qt::Horizontal, "Votes");
  playlistModel->setHeaderData(7, Qt::Horizontal, "Time Added");
  playlistModel->setEditStrategy(QSqlTableModel::OnFieldChange);
  horizontalHeader()->setStretchLastSection(true);
//  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setItemDelegate(new PlaylistDelegate(this));
  setModel(playlistModel);
  setColumnHidden(0,true);
  setColumnHidden(1,true);
  setColumnHidden(5,true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
}
  
void PlaylistView::addSongToPlaylist(const QModelIndex& libraryIndex){
  QString libraryId = musicLibrary->data(
    libraryIndex.sibling(libraryIndex.row(),0)).toString();
  QSqlQuery addQuery(
    "INSERT INTO mainplaylist (libraryId, timeAdded) VALUES ( ? , strftime('%s','now') )", 
    database);
  addQuery.addBindValue(QString(libraryId));
  addQuery.exec();
  playlistModel->select();
}

QString PlaylistView::getFilePath(const QModelIndex& songIndex) const{
  QModelIndex filePathIndex = songIndex.sibling(songIndex.row(), 5);
  return playlistModel->data(filePathIndex).toString();
}


} //end namespace

