/**
 * @file ASTNode.cpp
 * @brief Implementation of ASTNode class
 *
 * @author Michael Albers
 */

#include "ASTNode.h"
#include "Token.h"

//*****************
// ASTNode::ASTNode
//*****************
ASTNode::ASTNode(const std::string &theNodeText) :
  myNodeText{new std::string{theNodeText}},
  myParent(nullptr)
{
}

//*****************
// ASTNode::ASTNode
//*****************
ASTNode::ASTNode(const Token &theToken) :
  myToken{new Token{theToken}}
{
}

//******************
// ASTNode::addChild
//******************
ASTNode* ASTNode::addChild(const std::string &theNodeText)
{
  auto newNode = new ASTNode(theNodeText);
  addChild(newNode);
  return newNode;
}

//******************
// ASTNode::addChild
//******************
ASTNode* ASTNode::addChild(const Token &theToken)
{
  auto newNode = new ASTNode(theToken);
  addChild(newNode);
  return newNode;
}

//******************
// ASTNode::addChild
//******************
void ASTNode::addChild(ASTNode *theChild)
{
  theChild->myParent = this;
  myChildren.push_back(std::shared_ptr<ASTNode>{theChild});
}

//*********************
// ASTNode::getChildren
//*********************
std::vector<std::shared_ptr<ASTNode>> ASTNode::getChildren() const noexcept
{
  return myChildren;
}

//*******************
// ASTNode::getParent
//*******************
ASTNode* ASTNode::getParent() const noexcept
{
  return myParent;
}

//******************
// ASTNode::traverse
//******************
void ASTNode::traverse(std::ostream &theOS) const noexcept
{
  if (myChildren.empty())
  {
    if (myToken.get())
    {
      theOS << myToken->getTokenString();
    }
    else
    {
      theOS << *myNodeText;
    }
    theOS << " ";
  }
  else
  {
    for (auto child : myChildren)
    {
      child->traverse(theOS);
    }
  }
}
