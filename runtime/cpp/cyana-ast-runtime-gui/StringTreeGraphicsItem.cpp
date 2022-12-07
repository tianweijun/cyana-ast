#include "StringTreeGraphicsItem.h"

#include <QPen>

QFont *StringTreeGraphicsItem::font = 0;
QFontMetrics *StringTreeGraphicsItem::fontMetrics = 0;
int StringTreeGraphicsItem::fontHeight = 0;

int StringTreeGraphicsItem::colLineHeight = 0;
int StringTreeGraphicsItem::rowTextGap = 0;

StringTreeGraphicsItem::StringTreeGraphicsItem(const StringTree *stringTree)
    : stringTree(stringTree), width(600), height(600) {
  if (0 == StringTreeGraphicsItem::font) {
    StringTreeGraphicsItem::font = new QFont();
    StringTreeGraphicsItem::font->setPointSize(16);
    StringTreeGraphicsItem::fontMetrics =
        new QFontMetrics(*StringTreeGraphicsItem::font);
    StringTreeGraphicsItem::fontHeight = fontMetrics->height();
    StringTreeGraphicsItem::colLineHeight =
        StringTreeGraphicsItem::fontHeight * 2;
    StringTreeGraphicsItem::rowTextGap = fontHeight;
  }
  getDrawTreeContext(stringTree);
}

StringTreeGraphicsItem::~StringTreeGraphicsItem() {
  // delete StringTreeGraphicsItem::fontMetrics;
  // StringTreeGraphicsItem::fontMetrics = 0;
  // delete StringTreeGraphicsItem::font;
  // StringTreeGraphicsItem::font = 0;
}


QRectF StringTreeGraphicsItem::boundingRect() const {
  return QRectF(0 - width / 2.0, 0 - height / 2.0, width, height);
}

void StringTreeGraphicsItem::paint(QPainter *painter,
                                   const QStyleOptionGraphicsItem *option,
                                   QWidget *widget) {
  painter->setRenderHint(QPainter::Antialiasing);   //抗锯齿
  painter->setFont(*StringTreeGraphicsItem::font);  //设置字体

  //设置画笔
  QPen pen;
  pen.setWidth(1);                  //线宽
  pen.setColor(Qt::black);          //划线颜色
  pen.setStyle(Qt::SolidLine);      //线的类型，实线、虚线等
  pen.setCapStyle(Qt::FlatCap);     //线端点样式
  pen.setJoinStyle(Qt::BevelJoin);  //线的连接点样式
  painter->setPen(pen);

  if (0 == stringTree) {
    return;
  }
  BoxTreeContext *boxTreeContext = getDrawTreeContext(stringTree);
  drawBoxTree(painter, boxTreeContext, boxTreeContext->boxTree);
  delete boxTreeContext;
  boxTreeContext = 0;
}

void StringTreeGraphicsItem::drawBoxTree(QPainter *painter,
                                         BoxTreeContext *boxTreeContext,
                                         Box *box) {
  int leftTranslation = 0 - boxTreeContext->width / 2;
  int upTranslation = 0 - boxTreeContext->height / 2;
  if (!(0 == box->text || box->text->length() <= 0)) {
    painter->drawText(QPoint(box->horizontalAxis + leftTranslation,
                             box->verticalAxis + upTranslation),
                      QString::fromStdString(*(box->text)));
  }
  std::list<Box *> *children = box->children;
  if (children->size() > 0) {
    int fromOfLineX = box->horizontalAxis + box->width / 2;
    int fromOfLineY = box->verticalAxis;

    for (auto childIt = box->children->begin(); childIt != box->children->end();
         childIt++) {
      Box *child = *childIt;
      int toOfLineX = child->horizontalAxis + child->width / 2;
      int toOfLineY = child->verticalAxis - fontHeight;
      painter->drawLine(
          QPoint(fromOfLineX + leftTranslation, fromOfLineY + upTranslation),
          QPoint(toOfLineX + leftTranslation, toOfLineY + upTranslation));
    }
  }
  if (children->size() > 0) {
    for (auto childIt = box->children->begin(); childIt != box->children->end();
         childIt++) {
      Box *child = *childIt;
      drawBoxTree(painter, boxTreeContext, child);
    }
  }
}

BoxTreeContext *StringTreeGraphicsItem::getDrawTreeContext(
    const StringTree *strTree) {
  BoxTreeContext *boxTreeContext = new BoxTreeContext();
  boxTreeContext->build(strTree);
  refreshDrawingBoxTreeContext(boxTreeContext);
  return boxTreeContext;
}

void StringTreeGraphicsItem::refreshDrawingBoxTreeContext(
    BoxTreeContext *boxTreeContext) {
  this->width = boxTreeContext->width;
  this->height = boxTreeContext->height;
}

Box::Box(std::string *text, Box *parent, HierarchicalRow hierarchicalRow) {
  this->text = text;
  this->parent = parent;
  this->hierarchicalRow = hierarchicalRow;

  this->horizontalAxis = 0;
  this->verticalAxis = 0;
  this->children = new std::list<Box *>();
  this->height = StringTreeGraphicsItem::fontHeight;

  if (text == 0 || text->length() <= 0) {
    this->width = StringTreeGraphicsItem::fontHeight;
  } else {
    QString qText = QString::fromStdString(*text);
    QRect rec = StringTreeGraphicsItem::fontMetrics->boundingRect(qText);
    //字符串所占的像素宽度,高度
    this->width = rec.width();
  }
}

Box::~Box() {
  // delete by StringTree
  // delete text;
  // text = 0;

  // delete children
  if (children != 0) {
    for (std::list<Box *>::const_iterator childIt = children->begin();
         childIt != children->end();) {
      Box *child = *childIt;
      delete child;
      child = 0;
      childIt = children->erase(childIt);
    }
    delete children;
    children = 0;
  }
}

BoxTreeContext::BoxTreeContext() : width(0), height(0), boxTree(0) {
  hierarchicalRowMap = new std::map<HierarchicalRow, std::list<Box *> *>();
}

BoxTreeContext::~BoxTreeContext() {
  // delete hierarchicalRowMap
  if (hierarchicalRowMap != 0) {
    for (auto hierarchicalRowMapIt = hierarchicalRowMap->begin();
         hierarchicalRowMapIt != hierarchicalRowMap->end();
         hierarchicalRowMapIt++) {
      std::list<Box *> *row = hierarchicalRowMapIt->second;
      delete row;
      row = 0;
    }
    delete hierarchicalRowMap;
    hierarchicalRowMap = 0;
  }
  //删掉所有box
  delete boxTree;
  boxTree = 0;
}

void BoxTreeContext::build(const StringTree *stringTree) {
  hierarchicalRowMap->clear();
  boxTree = initBoxTree(stringTree, 0, HierarchicalRow(1));
  initLocationOfBox();
  alignCenter(boxTree);
  setWidthAndHeight();
}

Box *BoxTreeContext::initBoxTree(const StringTree *strTree, Box *parent,
                                 HierarchicalRow hierarchicalRow) {
  std::string *text = strTree->text;
  Box *box = new Box(text, parent, hierarchicalRow);
  addBoxToHierarchicalRowMap(hierarchicalRow, box);

  HierarchicalRow hierarchicalRowOfChildTree(hierarchicalRow.heightOfTree + 1);
  for (auto strTreeChildrenIt = strTree->children->begin();
       strTreeChildrenIt != strTree->children->end(); strTreeChildrenIt++) {
    StringTree *strTreeChild = *strTreeChildrenIt;
    Box *boxOfChild =
        initBoxTree(strTreeChild, box, hierarchicalRowOfChildTree);
    box->children->push_back(boxOfChild);
  }
  return box;
}

void BoxTreeContext::addBoxToHierarchicalRowMap(HierarchicalRow hierarchicalRow,
                                                Box *box) {
  auto findIt = hierarchicalRowMap->find(hierarchicalRow);
  std::list<Box *> *boxesInRow = 0;
  if (findIt == hierarchicalRowMap->end()) {
    boxesInRow = new std::list<Box *>();
    std::pair<HierarchicalRow, std::list<Box *> *> keyValue(hierarchicalRow,
                                                            boxesInRow);
    hierarchicalRowMap->insert(keyValue);
  } else {
    boxesInRow = findIt->second;
  }
  boxesInRow->push_back(box);
}

void BoxTreeContext::initLocationOfBox() {
  int margin = StringTreeGraphicsItem::fontHeight;
  int fontHeight = StringTreeGraphicsItem::fontHeight;
  int colLineHeight = StringTreeGraphicsItem::colLineHeight;
  int rowTextGap = StringTreeGraphicsItem::rowTextGap;
  for (auto hierarchicalRowMapIt = hierarchicalRowMap->begin();
       hierarchicalRowMapIt != hierarchicalRowMap->end();
       hierarchicalRowMapIt++) {
    HierarchicalRow *hierarchicalRow =
        const_cast<HierarchicalRow *>(&hierarchicalRowMapIt->first);
    std::list<Box *> *row = hierarchicalRowMapIt->second;
    int endInRow = margin;
    int verticalAxis =
        margin +
        (hierarchicalRow->heightOfTree - 1) * (fontHeight + colLineHeight) +
        fontHeight;

    for (auto rowIt = row->begin(); rowIt != row->end(); rowIt++) {
      Box *box = *rowIt;
      box->horizontalAxis = endInRow;
      box->verticalAxis = verticalAxis;

      endInRow += box->width;
      endInRow += rowTextGap;
    }
    hierarchicalRow->endOfRow = endInRow;
    hierarchicalRow->height = verticalAxis;
  }
}

void BoxTreeContext::alignCenter(Box *box) {
  if (box->children->empty()) {
    return;
  }
  for (auto childIt = box->children->begin(); childIt != box->children->end();
       childIt++) {
    Box *childBox = *childIt;
    alignCenter(childBox);
  }
  int startOfChildRow = box->children->front()->horizontalAxis;
  Box *childrenLast = box->children->back();
  int endOfChildRow = childrenLast->horizontalAxis + childrenLast->width;
  int midOfChildRow = startOfChildRow + (endOfChildRow - startOfChildRow) / 2;
  int midOfThis = box->horizontalAxis + box->width / 2;
  if (midOfThis < midOfChildRow) {  // 元素偏左，其之后的当前行box右移
    int thisMoveRight = midOfChildRow - midOfThis;
    moveRightBoxInRow(box, thisMoveRight);
  }
  if (midOfThis > midOfChildRow) {  // 元素偏右,孩子右移
    int childMoveRight = midOfThis - midOfChildRow;
    std::set<HierarchicalRow> hasMovedRows;
    for (auto childIt = box->children->begin(); childIt != box->children->end();
         childIt++) {
      Box *child = *childIt;
      moveRightOnceForEachRow(hasMovedRows, child, childMoveRight);
    }
  }
}

void BoxTreeContext::moveRightBoxInRow(Box *box, int moveRight) {
  bool shouldMoveRight = false;
  auto findRowIt = hierarchicalRowMap->find(box->hierarchicalRow);
  std::list<Box *> *boxesInRow = findRowIt->second;
  auto boxesInRowIt = boxesInRow->begin();
  while (boxesInRowIt != boxesInRow->end()) {
    Box *boxInRow = *boxesInRowIt;
    boxesInRowIt++;
    if (boxInRow == box) {
      shouldMoveRight = true;
      boxInRow->horizontalAxis += moveRight;
      break;
    }
  }
  if (shouldMoveRight) {
    while (boxesInRowIt != boxesInRow->end()) {
      Box *boxInRow = *boxesInRowIt;
      boxesInRowIt++;
      boxInRow->horizontalAxis += moveRight;
    }
    box->hierarchicalRow.endOfRow += moveRight;
    HierarchicalRow *hierarchicalRow =
        const_cast<HierarchicalRow *>(&findRowIt->first);
    hierarchicalRow->endOfRow += moveRight;
  }
}

void BoxTreeContext::moveRightOnceForEachRow(
    std::set<HierarchicalRow> &hasMovedRows, Box *box, int moveRight) {
  auto findHasMovedRowIt = hasMovedRows.find(box->hierarchicalRow);
  if (findHasMovedRowIt == hasMovedRows.end()) {
    moveRightBoxInRow(box, moveRight);
    hasMovedRows.insert(box->hierarchicalRow);
  }
  for (auto childIt = box->children->begin(); childIt != box->children->end();
       childIt++) {
    Box *child = *childIt;
    moveRightOnceForEachRow(hasMovedRows, child, moveRight);
  }
}

void BoxTreeContext::setWidthAndHeight() {
  int maxHeight = 0;
  int maxWidth = 0;
  for (auto hierarchicalRowMapIt = hierarchicalRowMap->begin();
       hierarchicalRowMapIt != hierarchicalRowMap->end();
       hierarchicalRowMapIt++) {
    HierarchicalRow *hierarchicalRow =
        const_cast<HierarchicalRow *>(&hierarchicalRowMapIt->first);
    if (hierarchicalRow->height > maxHeight) {
      maxHeight = hierarchicalRow->height;
    }
    if (hierarchicalRow->endOfRow > maxWidth) {
      maxWidth = hierarchicalRow->endOfRow;
    }
  }
  this->height =
      maxHeight + StringTreeGraphicsItem::fontHeight;  // margin-bottom
  this->width = maxWidth;
}
