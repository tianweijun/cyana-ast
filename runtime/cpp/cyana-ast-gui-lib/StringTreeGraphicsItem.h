#ifndef STRINGTREEGRAPHICSITEM_H
#define STRINGTREEGRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <list>
#include <map>
#include <set>

#include "StringTree.h"

class Box;
class BoxTreeContext;

class StringTreeGraphicsItem : public QGraphicsItem {
 public:
  StringTreeGraphicsItem(StringTree *stringTree);
  ~StringTreeGraphicsItem();

 public:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

 private:
  int width;
  int height;
  StringTree *stringTree;

 public:
  static QFont *font;
  static QFontMetrics *fontMetrics;
  static int fontHeight;
  static int colLineHeight;
  static int rowTextGap;

 private:
  void drawBoxTree(QPainter *painter, BoxTreeContext *boxTreeContext, Box *box);
  BoxTreeContext *getDrawTreeContext(StringTree *strTree);
  void refreshDrawingBoxTreeContext(BoxTreeContext *boxTreeContext);
};

class HierarchicalRow {
 public:
  HierarchicalRow() : heightOfTree(0), endOfRow(0), height(0) {}
  HierarchicalRow(int heightOfTree)
      : heightOfTree(heightOfTree), endOfRow(0), height(0) {}
  ~HierarchicalRow() {}
  int heightOfTree;
  int endOfRow;
  int height;

  bool operator<(const HierarchicalRow &o) const {
    return this->heightOfTree < o.heightOfTree;
  }

  // template <class T>
  // friend class HierarchicalRowCompare;
};

/*
template <class T>
class HierarchicalRowCompare {
 public:
  bool operator()(const T &t1, const T &t2) {
    return t1.heightOfTree < t2.heightOfTree;
  }
};
*/

class Box {
 public:
  Box(std::string *text, Box *parent, HierarchicalRow hierarchicalRow);
  ~Box();

  HierarchicalRow hierarchicalRow;
  int horizontalAxis;
  int verticalAxis;
  int width;
  int height;
  std::string *text;
  Box *parent;
  std::list<Box *> *children;
};

class BoxTreeContext {
 public:
  BoxTreeContext();
  ~BoxTreeContext();

  int width;
  int height;
  std::map<HierarchicalRow, std::list<Box *> *> *hierarchicalRowMap;
  Box *boxTree;

  void build(StringTree *stringTree);

 private:
  Box *initBoxTree(StringTree *tree, Box *parent,
                   HierarchicalRow hierarchicalRow);
  void addBoxToHierarchicalRowMap(HierarchicalRow hierarchicalRow, Box *box);
  void initLocationOfBox();
  void alignCenter(Box *box);
  void moveRightBoxInRow(Box *box, int moveRight);
  void moveRightOnceForEachRow(std::set<HierarchicalRow> &hasMovedRows,
                               Box *box, int moveRight);
  void setWidthAndHeight();
};

#endif  // STRINGTREEGRAPHICSITEM_H
