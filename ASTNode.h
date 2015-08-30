#ifndef ASTNODE_H
#define ASTNODE_H

/**
 * @file ASTNode.h
 * @brief Defines a single node within an AST.
 *
 * @author Michael Albers
 */

#include <memory>
#include <ostream>
#include <string>
#include <vector>

class Token;

/**
 * Defines a node within an abstract syntax tree. Nodes are self-referential,
 * thus a each node is the root of a sub-tree (with one node being the
 * actual root of the tree).
 *
 * Each node owns each pointer to its children. In other words the caller
 * can freely 'new' objects for parameters to addChild.
 */
class ASTNode
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  ASTNode() = delete;

  /**
   * Copy constructor.
   */
  ASTNode(const ASTNode &) = default;

  /**
   * Move constructor.
   */
  ASTNode(ASTNode &&) = default;

  /**
   * Constructor.
   *
   * @param theNodeText
   *          Text for the value of the node. Typically this constructor
   *          would be used for non-terminals.
   */
  ASTNode(const std::string &theNodeText);

  /**
   * Constructor.
   *
   * @param theToken
   *          Token being parsed.
   */
  ASTNode(const Token &theToken);

  /**
   * Destructor
   */
  ~ASTNode() = default;

  /**
   * Copy assignment operator.
   */
  ASTNode& operator=(const ASTNode &) = default;

  /**
   * Move assignment operator.
   */
  ASTNode& operator=(ASTNode &&) = default;

  /**
   * Adds a child node with the given text to this node.
   *
   * @param theNodeText
   *          node text
   * @return Pointer to child node (this should not be deleted by caller).
   */
  ASTNode* addChild(const std::string &theNodeText);

  /**
   * Adds a child node with the given token to this node.
   *
   * @param theToken
   *          node token
   * @return Pointer to child node (this should not be deleted by caller).
   */
  ASTNode* addChild(const Token &theToken);

  /**
   * Adds the given node as a child of this node. This class takes ownership
   * of the given node.
   *
   * @param theChild
   *          child to add
   */
  void addChild(ASTNode *theChild);

  /**
   * Returns all of the child nodes.
   *
   * @return child nodes
   */
  std::vector<std::shared_ptr<ASTNode>> getChildren() const noexcept;

  /**
   * Returns the parent node of this node.
   *
   * @return Pointer to the parent node (this should not be deleted by caller).
   */
  ASTNode* getParent() const noexcept;

  /**
   * Prints node values for all leaf nodes, in a left-to-right order.
   *
   * @param theOS
   *          output stream
   */
  void traverse(std::ostream &theOS) const noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Node text, null if token is used. */
  std::shared_ptr<std::string> myNodeText;

  /** Node token, null if text is used. */
  std::shared_ptr<Token> myToken;

  /** Parent node. */
  ASTNode *myParent;

  /** All child nodes. */
  std::vector<std::shared_ptr<ASTNode>> myChildren;
};

#endif
