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

#include <DeintFilter.hpp>

#include <QSharedPointer>
#include <QWaitCondition>
#include <QVector>
#include <QThread>
#include <QMutex>

class YadifDeint;

class YadifThr : public QThread
{
public:
	YadifThr(const YadifDeint &yadifDeint);
	~YadifThr() final;

	void start(VideoFrame &destFrame, const VideoFrame &prevFrame, const VideoFrame &currFrame, const VideoFrame &nextFrame, const int id, const int n);
	void waitForFinished();
private:
	void run() override final;

	const YadifDeint &yadifDeint;

	VideoFrame *dest;
	const VideoFrame *prev, *curr, *next;
	int jobId, jobsCount;
	bool hasNewData, br;

	QWaitCondition cond;
	QMutex mutex;
};

class YadifDeint : public DeintFilter
{
	friend class YadifThr;
public:
	YadifDeint(bool doubler, bool spatialCheck);

	void clearBuffer() override final;

	bool filter(QQueue<FrameBuffer> &framesQueue) override final;

	bool processParams(bool *paramsCorrected) override final;
private:
	inline void doFilter(VideoFrame &dest, const VideoFrame &prev, const VideoFrame &curr, const VideoFrame &next, const int id, const int jobsCount) const;

	using YadifThrPtr = QSharedPointer<YadifThr>;
	QVector<YadifThrPtr> threads;

	const bool doubler, spatialCheck;
	bool secondFrame;
};

#define YadifDeintName "Yadif"
#define YadifNoSpatialDeintName YadifDeintName " (no spatial check)"
#define Yadif2xDeintName YadifDeintName " 2x"
#define Yadif2xNoSpatialDeintName Yadif2xDeintName " (no spatial check)"

#define YadifDescr "Yet Another DeInterlacong Filter"
