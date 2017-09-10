/*
	QMPlay2 is a video and audio player.
	Copyright (C) 2010-2017  Błażej Szczygieł

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QMPlay2Extensions.hpp>
#include <VisWidget.hpp>

#include <QCoreApplication>
#include <QLinearGradient>

class SimpleVis;

class SimpleVisW : public VisWidget
{
	friend class SimpleVis;

	Q_DECLARE_TR_FUNCTIONS(SimpleVisW)
public:
	SimpleVisW(SimpleVis &);
private:
	void paint(QPainter &p) override final;

	void resizeEvent(QResizeEvent *) override final;

	void start(bool v = false) override final;
	void stop() override final;

	QByteArray soundData;
	quint8 chn;
	quint32 srate;
	int interval;
	qreal leftBar, rightBar;
	QPair<qreal, double> leftLine, rightLine;
	SimpleVis &simpleVis;
	QLinearGradient linearGrad;
	bool fullScreen;
};

/**/

class SimpleVis : public QMPlay2Extensions
{
public:
	SimpleVis(Module &);

	void soundBuffer(const bool);

	bool set() override final;
private:
	DockWidget *getDockWidget() override final;

	bool isVisualization() const override final;
	void connectDoubleClick(const QObject *, const char *) override final;
	void visState(bool, uchar, uint) override final;
	void sendSoundData(const QByteArray &) override final;
	void clearSoundData() override final;

	/**/

	SimpleVisW w;

	QByteArray tmpData;
	int tmpDataPos;
	QMutex mutex;
	float sndLen;
};

#define SimpleVisName "Prosta wizualizacja"
