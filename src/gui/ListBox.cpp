/*!
    @file   ListBox.cpp
    @author dasyprocta
 */

#include "ListBox.h"
#include "Graphics.hpp"
#include <boost/foreach.hpp>
#include <algorithm>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
ListBox::Ptr ListBox::Create(
    ComponentId                         id,
    Arg const&                          arg)
{
    ListBox::Ptr p(new ListBox);
    p->m_arg = arg;
    p->setLocation(Rect<sint32>(0, 0, 0, arg.m_height));

    p->createScrollBar(
        p,
        &ScrollBar::Arg().disable(false),
        &ScrollBar::Arg().disable(true));

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
sint32 ListBox::getSelectedIndex() const
{
    return m_selectedIndex;
}

// ----------------------------------------------------------------------------
void ListBox::setSelectedIndex(sint32 index)
{
    m_selectedIndex = index;
}

// ----------------------------------------------------------------------------
void ListBox::draw(Graphics& graphics)
{
    ComponentGraphics g(graphics);

    Rect<sint32> region = location_;
    Rect<sint32> client;
    getScrolledClientRect(client);

    // bound line
    g.drawEdit(region);

    // column label
    int columnX = 0;
    for (int i = 0; i < m_columnLabels.size(); ++i)
    {
        bool breakFlag = false;
        int right = client.left + columnX + m_columnLabels[i].width;
        if (right >= (client.right))
        {
            right = client.right;
            breakFlag = true;
        }

        Rect<sint32> column(client.left + columnX, client.top-Column::Height, right, client.top);
        g.drawFrame(column, true, true);

        Rect<sint32> clip = column;
        clip.right -= 2;
        graphics.drawText(
            Point<sint32>(region.left + columnX + 3, region.top + 3),
            m_columnLabels[i].label, 0xffeeeeee, clip);
        if (breakFlag) break;
        else
        {
            int height = client.getHeight() - 20;
            graphics.setColor(0xff333333);
            graphics.drawLine(Point<sint32>(right-1, region.top+1+Column::Height), Point<sint32>(right-1, region.bottom-1));
        }

        columnX += m_columnLabels[i].width;
    }

    // client
    if (m_items.size() == 0) return;

    Size clientWzScrollBar;
    getClientSizeWithScrollBar(clientWzScrollBar);

    int clientY = client.top;
    int clientX = client.left;
    int beginRow = getScrollBar(ScrollBar::Vertical)->getContentOffset() / 18;
    int restY = getScrollBar(ScrollBar::Vertical)->getContentOffset() % 18;
    clientY -= restY;
    int endRow = std::min(beginRow + (clientWzScrollBar.height+restY) / 18+1, static_cast<int>(m_items.size()));

    ItemMatrix::const_iterator it = m_items.begin();
    for (int i = 0; i < beginRow; ++i) ++it;
    for (int i = beginRow; i < endRow; ++i, ++it)
    {
        clientX = client.left;
        for (int j = 0; j < m_columnLabels.size(); ++j)
        {
            bool breakFlag = false;
            int right = clientX + m_columnLabels[j].width;
            if (right >= client.left + clientWzScrollBar.width)
            {
                right = client.left + clientWzScrollBar.width;
                breakFlag = true;
            }
            // è„ã´äE
            int columnY = (clientY < client.top)? clientY+restY: clientY;
            Rect<sint32> column(clientX, columnY, right, clientY+18);
            // â∫ã´äE
            if (clientY+18 >= client.top + clientWzScrollBar.height)
                column.bottom = client.top + clientWzScrollBar.height;

            std::string str = (*it)[j].get<std::string>();
            graphics.drawText(Point<sint32>(clientX + 2, clientY + 2), str, 0xffeeeeee, column);

            clientX += m_columnLabels[j].width;
            if (breakFlag) break;
        }
        // line
        clientY += 18;
        if (clientY >= client.top + clientWzScrollBar.height) break;
        graphics.setColor(0xff333333);
        graphics.drawLine(Point<sint32>(client.left, clientY-1), Point<sint32>(client.right-1, clientY-1));
    }

    // scroll bar
    drawScrollBar(graphics);
}

// ----------------------------------------------------------------------------
void ListBox::update()
{
    updateScrollBar();
    Component::update();
}

// ----------------------------------------------------------------------------
int ListBox::getHeight() const
{
    return m_arg.m_height;
}

// ----------------------------------------------------------------------------
sint32 ListBox::calcClientWidth() const
{
    sint32 width = 0;
    foreach (Column const& label, m_columnLabels)
    {
        width += label.width;
    }
    return width;
}

// ----------------------------------------------------------------------------
void ListBox::getScrolledClientRect(Rect<sint32>& rect) const
{
    rect = location_;
    rect.left   += 1;
    rect.top    += Column::Height + 1;
    rect.right  -= 1;
    rect.bottom -= 1;
}

// ----------------------------------------------------------------------------
void ListBox::getScrolledContentSize(Size& size) const
{
    size.width  = calcClientWidth();
    size.height = m_items.size() * 18;
}

// ----------------------------------------------------------------------------
void ListBox::getScrolledUnitInclementSize(Size& size) const
{
    size.width  = 18;
    size.height = 18;
}

#if 0

// ----------------------------------------------------------------------------
sint32 ListBox::getClientSize(ScrollBar::Type type) const
{
    sint32 size;
    if (type == ScrollBar::Vertical)
    {
        ScrollBar::Ptr bar = getScrollBar(ScrollBar::Horizontal);
        size = location_.getHeight()-Column::Height-2-(bar->isVisible()? bar->getHeight(): 0);
    }
    else if (type == ScrollBar::Horizontal)
    {
        ScrollBar::Ptr bar = getScrollBar(ScrollBar::Vertical);
        size = location_.getWidth()-2-(bar->isVisible()? bar->getWidth(): 0);
    }
    else assert(0);

    return size;
}

// ----------------------------------------------------------------------------
sint32 ListBox::getContentSize(ScrollBar::Type type) const
{
    sint32 size;
    if (type == ScrollBar::Vertical)
        size = m_items.size() * 18;
    else if (type == ScrollBar::Horizontal)
        size = calcClientWidth();
    else assert(0);

    return size;
}

// ----------------------------------------------------------------------------
sint32 ListBox::getUnitInclementSize(ScrollBar::Type type) const
{
    if (type == ScrollBar::Vertical)
        return 18;
    else if (type == ScrollBar::Horizontal)
        return 18;
    else assert(0);
}

// ----------------------------------------------------------------------------
bool ListBox::isVHScrollEnabled() const
{
    return getScrollBar(ScrollBar::Horizontal)->isVisible() &&
           getScrollBar(ScrollBar::Vertical)->isVisible();
}

#endif

// ----------------------------------------------------------------------------
bool ListBox::onMouseProcess(MouseCommand const& mouse)
{
    if (onScrollBarMouseProcess(mouse))
    {
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
bool ListBox::onKeyboardProcess(KeyboardCommand const& keyboard)
{
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
