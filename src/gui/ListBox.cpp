/*!
    @file   ListBox.cpp
    @author dasyprocta
 */

#include "ListBox.h"
#include "Graphics.hpp"
#include <algorithm>

#undef min
#undef max

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
ListBox::Ptr ListBox::Create(
    ComponentId                         id,
    Arg const&                          arg)
{
    ListBox::Ptr p(new ListBox);
    p->m_arg = arg;
    p->setLocation(Rect(0, 0, 0, arg.m_height));

    p->m_scrollBar = ScrollBar::Create(-1, ScrollBar::Arg().owner(p).type(ScrollBar::Vertical));

    return p;
}

// ----------------------------------------------------------------------------
ListBox& ListBox::addColumn(
    std::string const&                  label,
    sint32                              width,
    sint32                              column)
{
    if (column < 0)
    {
        m_columnLabels.push_back(Column(width, label));
    }
    return *this;
}

// ----------------------------------------------------------------------------
ListBox& ListBox::addRow(
    sint32                              row)
{
    if (row < 0)
    {
        ItemVector rowItems;
        rowItems.resize(m_columnLabels.size());
        for (int i = 0; i < m_columnLabels.size(); ++i)
        {
            rowItems[i].set(std::string(""));
        }
        m_items.push_back(rowItems);
    }
    return *this;
}

// ----------------------------------------------------------------------------
ListBox::Item& ListBox::getItem(
    sint32                              row,
    sint32                              column) const
{
    ItemMatrix::const_iterator it = m_items.begin();
    for (int i = 0; i < row; ++i) ++it;
    Item const& item = (*it)[column];
    return const_cast<Item&>(item);
}

// ----------------------------------------------------------------------------
void ListBox::draw(Graphics& graphics)
{
    Rect region = location_;

    // bound line
    graphics.setColor(0xff222222);
    graphics.drawLine(Point(region.left, region.top), Point(region.right, region.top));
    graphics.drawLine(Point(region.left, region.top), Point(region.left, region.bottom-1));

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(region.right, region.top+1), Point(region.right, region.bottom));
    graphics.drawLine(Point(region.left, region.bottom), Point(region.right, region.bottom));

    // column label
    graphics.setRectColor(0xff777777, 0xff777777, 0xff333333, 0xff333333);
    int columnX = 1;
    for (int i = 0; i < m_columnLabels.size(); ++i)
    {
        bool breakFlag = false;
        int right = region.left + columnX + m_columnLabels[i].width;
        if (right > (region.right - m_scrollBar->getWidth() - 1))
        {
            right = location_.right - m_scrollBar->getWidth() - 1;
            breakFlag = true;
        }

        Rect column(region.left + columnX, region.top+1, right-1, region.top+1+18);
        graphics.drawFillRect(column);

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(region.left + columnX, region.top+1), Point(right-1, region.top+1));
        graphics.drawLine(Point(region.left + columnX, region.top+1), Point(region.left + columnX, region.top+18-1));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(right-1, region.top+2), Point(right-1, region.top+18+1));
        graphics.drawLine(Point(region.left + columnX, region.top+18+1), Point(right-1, region.top+18+1));

        Rect clip = column;
        clip.right -= 2;
        graphics.drawText(
            Point(region.left + columnX + 3, region.top + 3),
            m_columnLabels[i].label, 0xffeeeeee, clip);
        if (breakFlag) break;
        else
        {
            int height = getClientWidth() - 20;
            graphics.setColor(0xff333333);
            graphics.drawLine(Point(right-1, region.top + 19), Point(right-1, region.bottom - 1));
        }

        columnX += m_columnLabels[i].width;
    }

    // client
    if (m_items.size() == 0) return;

    int clientY = region.top + 19;
    int clientX = region.left + 1;
    int scrollBarWidth = m_scrollBar->getWidth();
    Rect clientRegion(clientX, clientY, region.right-1-scrollBarWidth, region.bottom-1);
    int beginRow = getScrollBarOffset() / 18;
    int restY = getScrollBarOffset() % 18;
    clientY -= restY;
    int endRow = beginRow + getClientWidth() / 18 + 1;

    ItemMatrix::const_iterator it = m_items.begin();
    for (int i = 0; i < beginRow; ++i) ++it;
    for (int i = beginRow; i < endRow; ++i, ++it)
    {
        clientX = region.left + 1;
        for (int j = 0; j < m_columnLabels.size(); ++j)
        {
            bool breakFlag = false;
            int right = clientX + m_columnLabels[j].width;
            if (right > (region.right - m_scrollBar->getWidth() - 1))
            {
                right = location_.right - m_scrollBar->getWidth() - 1;
                breakFlag = true;
            }
            int columnY = (clientY < region.top+19)? clientY+restY+2: clientY;
            Rect column(clientX, columnY, right-3, clientY+1+18);
            if (clientY+18 > region.bottom) column.bottom = region.bottom - 1;

            std::string str = (*it)[j].get<std::string>();
            graphics.drawText(Point(clientX + 2, clientY + 2), str, 0xffeeeeee, column);

            clientX += m_columnLabels[j].width;
            if (breakFlag) break;
        }
        // line
        clientY += 18;
        graphics.setColor(0xff333333);
        graphics.drawLine(Point(clientRegion.left, clientY), Point(clientRegion.right, clientY));
    }
    // scroll bar
    m_scrollBar->draw(graphics);
}

// ----------------------------------------------------------------------------
void ListBox::update()
{
    m_scrollBar->setX(location_.right - 11);
    m_scrollBar->setY(location_.top + 20);

    m_scrollBar->update();

    Component::update();
}

// ----------------------------------------------------------------------------
int ListBox::getHeight() const
{
    return m_arg.m_height;
}

// ----------------------------------------------------------------------------
sint32 ListBox::getClientWidth()
{
    // height
    int height = location_.getHeight() - 20;
    return height;
}

// ----------------------------------------------------------------------------
sint32 ListBox::getContentWidth()
{
    // height
    int numRows = m_items.size();
    int height = numRows * 18;
    return height;
}

// ----------------------------------------------------------------------------
sint32 ListBox::getOneContentSize()
{
    return 18;
}

// ----------------------------------------------------------------------------
sint32 ListBox::getNumContents()
{
    return m_items.size();
}

// ----------------------------------------------------------------------------
bool ListBox::onMouseProcess(MouseCommand const& mouse)
{
    if (m_scrollBar->onMouseProcess(mouse))
    {
        return true;
    }
    return false;
}



BEGIN_NO_NAMESPACE

void testListBox()
{
    ListBox::Ptr listBox = ListBox::Create(-1, ListBox::Arg().width(200));
    listBox->
        addColumn("Col 1", 100).
        addColumn("Col 2", 100).
        addColumn("Col 3", 120);
}

END_NO_NAMESPACE

END_NAMESPACE_NYANCO_GUI
