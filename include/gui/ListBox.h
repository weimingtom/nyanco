#pragma once

/*!
    @file   ListBox.h
    @author dasyprocta
 */

#include "Component.h"
#include "ScrollBar.h"
#include <vector>
#include <list>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

// ============================================================================
class ListBox : public Component, public ScrollBar::Owner
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(ListBox);

    struct Column
    {
        sint32                          width;
        std::string                     label;

        Column(sint32 width_, std::string const& label_)
            : width(width_), label(label_) {}
        Column() {}

        static sint32 const Height = 16;
    };

    struct Item
    {
    public:
        typedef boost::variant<sint32, float32, std::string> Var;

        template <typename Type_>
        void set(Type_ var) { m_var = var; }

        template <typename Type_>
        Type_ get() const { return boost::get<Type_>(m_var); }

        template <>
        std::string const& get() const
        {
            if (m_var.which() == 0)
            {
                sint32 var = boost::get<sint32>(m_var);
                return boost::lexical_cast<std::string>(var);
            }
            else if (m_var.which() == 1)
            {
                sint32 var = boost::get<float32>(m_var);
                return boost::lexical_cast<std::string>(var);
            }
            else if (m_var.which() == 2)
            {
                return boost::get<std::string>(m_var);
            }
            return "";
        }

    private:
        Var                             m_var;
    };

    struct Arg
    {
        Arg& width(sint32 width) { m_width = width; return *this; }
        Arg& height(sint32 height) { m_height = height; return *this; }
        sint32                          m_width;
        sint32                          m_height;
    };

    static ListBox::Ptr Create(
        ComponentId                     id,
        Arg const&                      arg);

    ListBox& addColumn(std::string const& label, sint32 width, sint32 column = -1);
    ListBox& addRow(sint32 row = -1);

    Item& getItem(sint32 row, sint32 column = 0) const;
    sint32 getSelectedIndex() const;
    void setSelectedIndex(sint32 index);

private:
    virtual void draw(Graphics& graphics);
    virtual void update();
    virtual int getHeight() const;
    virtual bool onMouseProcess(MouseCommand const& mouse);
    virtual bool onKeyboardProcess(KeyboardCommand const& keyboard);

    typedef std::vector<Item>           ItemVector;
    typedef std::list<ItemVector>       ItemMatrix;
    typedef std::vector<Column>         ColumnVector;

    ColumnVector                        m_columnLabels;
    ItemMatrix                          m_items;
    sint32                              m_selectedIndex;
    Arg                                 m_arg;

    ListBox() : m_selectedIndex(-1) {}

    // helper
    sint32 calcClientWidth() const;

    // scroll bar owner's implement
    void getScrolledClientRect(Rect& rect) const;
    void getScrolledContentSize(Size& size) const;
    void getScrolledUnitInclementSize(Size& size) const;
    /*
    sint32 getClientSize(ScrollBar::Type type) const;
    sint32 getContentSize(ScrollBar::Type type) const;
    sint32 getUnitInclementSize(ScrollBar::Type type) const;
    bool isVHScrollEnabled() const;
    */
};

END_NAMESPACE_NYANCO_GUI
