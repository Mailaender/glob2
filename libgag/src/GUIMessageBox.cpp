/*
  Copyright (C) 2001, 2002 Stephane Magnenat & Luc-Olivier de Charri?re
  for any question or comment contact us at nct@ysagoon.com or nuage@ysagoon.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <GUIMessageBox.h>
#include <GUIBase.h>
#include <GUIText.h>
#include <GUIButton.h>

class MessageBoxScreen:public OverlayScreen
{
public:
	MessageBoxScreen(GraphicContext *parentCtx, Font *font, MessageBoxType type, const char *title, int titleWidth, int totCaptionWidth, int captionCount, int captionWidth[3], const char *captionArray[3]);
	virtual ~MessageBoxScreen() { }
	virtual void onAction(Widget *source, Action action, int par1, int par2);
};

MessageBoxScreen::MessageBoxScreen(GraphicContext *parentCtx, Font *font, MessageBoxType type, const char *title, int titleWidth, int totCaptionWidth, int captionCount, int captionWidth[3], const char *captionArray[3])
:OverlayScreen(parentCtx, titleWidth > totCaptionWidth ? titleWidth : totCaptionWidth, 100)
{
	int w=titleWidth > totCaptionWidth ? titleWidth : totCaptionWidth;
	addWidget(new Text(0, 20, font, title, w));

	int dec;
	if (titleWidth>totCaptionWidth)
		dec=20+((titleWidth-totCaptionWidth)>>1);
	else
		dec=20;
	for (int i=0; i<captionCount; i++)
	{
		addWidget(new TextButton(dec, 50, captionWidth[i], 30, NULL, -1, -1, font, captionArray[i], i));
		dec+=20 + captionWidth[i];
	}
}

void MessageBoxScreen::onAction(Widget *source, Action action, int par1, int par2)
{
	if (action==BUTTON_PRESSED)
		endValue=par1;
}

int MessageBox(GraphicContext *parentCtx, Font *font, MessageBoxType type, const char *title, const char *caption1, const char *caption2, const char *caption3)
{
	// for passing captions to class
	const char *captionArray[3]={
		caption1,
		caption2,
		caption3 };

	int captionWidth[3];
	memset(captionWidth, 0, sizeof(captionWidth));

	// compute number of caption
	unsigned captionCount;
	if (caption3!=NULL)
	{
		captionCount = 3;
		captionWidth[2] = font->getStringWidth(captionArray[2])+10;
		captionWidth[1] = font->getStringWidth(captionArray[1])+10;
		captionWidth[0] = font->getStringWidth(captionArray[0])+10;
	}
	else if (caption2!=NULL)
	{
		captionCount = 2;
		captionWidth[1] = font->getStringWidth(captionArray[1])+10;
		captionWidth[0] = font->getStringWidth(captionArray[0])+10;
	}
	else
	{
		captionCount = 1;
		captionWidth[0] = font->getStringWidth(captionArray[0])+10;
	}

	int totCaptionWidth = captionWidth[0]+captionWidth[1]+captionWidth[2]+(captionCount-1)*20+40;
	int titleWidth =  font->getStringWidth(title)+10;

	MessageBoxScreen *mbs = new MessageBoxScreen(parentCtx, font, type, title, titleWidth, totCaptionWidth, captionCount, captionWidth, captionArray);

	mbs->dispatchPaint(mbs->getSurface());

	// save screen
	parentCtx->setClipRect();

	SDL_Event event;
	while(mbs->endValue<0)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type==SDL_QUIT)
				break;
			mbs->translateAndProcessEvent(&event);
		}
		parentCtx->drawSurface(mbs->decX, mbs->decY, mbs->getSurface());
		parentCtx->updateRect(mbs->decX, mbs->decY, mbs->getW(), mbs->getH());
	}

	int retVal;
	if (mbs->endValue>=0)
		retVal=mbs->endValue;
	else
		retVal=-1;

	// clean up
	delete mbs;

	return retVal;
}
