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
class ListBox
    : public Component,
      public ScrollBar::Owner
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

    virtual void draw(Graphics& graphics);
    virtual void update();
    virtual int getHeight() const;
    virtual bool onMouseProcess(MouseCommand const& mouse);

    virtual sint32 getClientWidth();
    virtual sint32 getContentWidth();
    virtual sint32 getOneContentSize();
    virtual sint32 getNumContents();

private:
    typedef std::vector<Item>           ItemVector;
    typedef std::list<ItemVector>       ItemMatrix;
    typedef std::vector<Column>         ColumnVector;

    ColumnVector                        m_columnLabels;
    ItemMatrix                          m_items;
    Arg                                 m_arg;

    ScrollBar::Ptr                      m_scrollBar;
};

END_NAMESPACE_NYANCO_GUI
