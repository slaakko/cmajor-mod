// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.container;

import cmajor.debugger;
import soul.xml.dom;
import util;
import std.core;
import cmajor.debug;

export namespace cmajor::debugger {

class Container;
class Debugger;

class Pointer
{
public:
    Pointer(Container& container_);
    Pointer(Container& container_, const std::string& expression_);
    bool IsNull();
    const std::string& Expression() const { return expression; }
    uint64_t Address();
    cmajor::debug::DIType* Type();
    std::unique_ptr<soul::xml::Element> Value();
    std::unique_ptr<soul::xml::Element> Deref();
    Pointer* PtrMember(const std::string& name);
    std::unique_ptr<soul::xml::Element> ValueMember(const std::string& name);
    std::string ValueMemberStr(const std::string& name);
private:
    Container& container;
    std::string expression;
    bool hasAddress;
    uint64_t address;
};

class Iterator
{
public:
    Iterator(Container& container_, int64_t index_);
    virtual ~Iterator();
    Container& GetContainer() { return container; }
    int64_t Index() const { return index; }
    bool IsEnd() const { return index == -1; }
    bool Equals(Iterator* that) const { return index == that->index; }
    virtual std::unique_ptr<soul::xml::Element> Current() = 0;
    virtual std::string CurrentStr() = 0;
    virtual Iterator* Next() = 0;
private:
    Container& container;
    int64_t index;
};

class ForwardListIterator : public Iterator
{
public:
    ForwardListIterator(Container& container, int64_t index, Pointer* nodePtr_);
    std::unique_ptr<soul::xml::Element> Current() override;
    std::string CurrentStr() override;
    Iterator* Next() override;
private:
    Pointer* nodePtr;
};

class LinkedListIterator : public Iterator
{
public:
    LinkedListIterator(Container& container, int64_t index, Pointer* nodePtr_);
    std::unique_ptr<soul::xml::Element> Current() override;
    std::string CurrentStr() override;
    Iterator* Next() override;
private:
    Pointer* nodePtr;
};

class HashtableIterator : public Iterator
{
public:
    HashtableIterator(Container& container, int64_t index, Pointer* bucketPtr_, int64_t bucketIndex_, int64_t bucketCount_, const std::string& bucketsExpr_);
    std::unique_ptr<soul::xml::Element> Current() override;
    std::string CurrentStr() override;
    Iterator* Next() override;
private:
    Pointer* bucketPtr;
    int64_t bucketIndex;
    int64_t bucketCount;
    std::string bucketsExpr;
};

class TreeIterator : public Iterator
{
public:
    TreeIterator(Container& container, int64_t index, Pointer* nodePtr_);
    std::unique_ptr<soul::xml::Element> Current() override;
    std::string CurrentStr() override;
    Iterator* Next() override;
private:
    Pointer* nodePtr;
};

class ListIterator : public Iterator
{
public:
    ListIterator(Container& container, int64_t index, int64_t count, Pointer* itemsPtr_);
    std::unique_ptr<soul::xml::Element> Current() override;
    std::string CurrentStr() override;
    Iterator* Next() override;
private:
    Pointer* itemsPtr;
    int64_t count;
};

uint64_t GetContainerAddress(cmajor::debug::ContainerClassTemplateKind containerKind, const std::string& containerVarExpr, Debugger* debugger);
Container* CreateContainer(cmajor::debug::ContainerClassTemplateKind containerKind, uint64_t address, Debugger* debugger);

class Container
{
public:
    Container(cmajor::debug::ContainerClassTemplateKind kind_, Debugger* debugger_, uint64_t address_);
    virtual ~Container();
    Container(const Container&) = delete;
    Container(Container&&) = delete;
    Container& operator=(const Container&) = delete;
    Container& operator=(Container&&) = delete;
    virtual void ClearBrowsingData();
    virtual int64_t Count(const std::string& expr);
    virtual Iterator* Begin(const std::string& containerVarExpr) = 0;
    virtual Iterator* End(const std::string& containerVarExpr) = 0;
    virtual std::unique_ptr<soul::xml::Element> Subscript(const std::string& containerVarExpr, int64_t index) = 0;
    virtual std::string SubscriptExpressionString(const std::string& containerVarExpr, int64_t index) = 0;
    std::unique_ptr<soul::xml::Element> Range(const std::string& containerVarExpr, int64_t rangeStart, int64_t rangeEnd);
    cmajor::debug::ContainerClassTemplateKind GetKind() const { return kind; }
    uint64_t Address() const { return address; }
    Pointer* GetPointer(uint64_t address) const;
    void AddPointer(Pointer* ptr);
    Debugger* GetDebugger() { return debugger; }
    Iterator* GetIterator(int64_t index) const;
    void AddIterator(Iterator* iterator);
    int64_t GetCount() { return count; }
    void SetCount(int64_t count_) { count = count_; }
    cmajor::debug::DIType* ValueNodePtrType() const { return valueNodePtrType; }
    void SetValueNodePtrType(cmajor::debug::DIType* type) { valueNodePtrType = type; }
    Pointer* EndPtr() const { return endPtr; }
    void SetEndPtr(Pointer* endPtr_) { endPtr = endPtr_; }
private:
    cmajor::debug::ContainerClassTemplateKind kind;
    Debugger* debugger;
    uint64_t address;
    int64_t count;
    cmajor::debug::DIType* valueNodePtrType;
    Pointer* endPtr;
    std::unordered_map<uint64_t, Pointer*> pointerMap;
    std::vector<std::unique_ptr<Pointer>> pointers;
    std::unordered_map<int64_t, Iterator*> iteratorMap;
    std::vector<std::unique_ptr<Iterator>> iterators;
};

class ForwardContainer : public Container
{
public:
    ForwardContainer(Debugger* debugger, cmajor::debug::ContainerClassTemplateKind kind, uint64_t address);
    std::unique_ptr<soul::xml::Element> Subscript(const std::string& containerVarExpr, int64_t index) override;
    std::string SubscriptExpressionString(const std::string& containerVarExpr, int64_t index) override;
};

class ListContainer : public ForwardContainer
{
public:
    ListContainer(Debugger* debugger, cmajor::debug::ContainerClassTemplateKind kind, uint64_t address);
    int64_t Count(const std::string& containerVarExpr) override;
};

class HashtableContainer : public ForwardContainer
{
public:
    HashtableContainer(Debugger* debugger, cmajor::debug::ContainerClassTemplateKind kind, uint64_t address);
    void ClearBrowsingData() override;
    void Init();
    int64_t Count(const std::string& containerVarExpr) override;
    Iterator* Begin(const std::string& containerVarExpr) override;
    Iterator* End(const std::string& containerVarExpr) override;
private:
    std::unique_ptr<Pointer> nullBucketPtr;
    int64_t bucketCount;
};

class TreeContainer : public ForwardContainer
{
public:
    TreeContainer(Debugger* debugger, cmajor::debug::ContainerClassTemplateKind kind, uint64_t address);
    void ClearBrowsingData() override;
    void Init(const std::string& containerVarExpr);
    int64_t Count(const std::string& containerVarExpr) override;
    Iterator* Begin(const std::string& containerVarExpr) override;
    Iterator* End(const std::string& containerVarExpr) override;
private:
    std::unique_ptr<Pointer> headerPtr;
};

class ForwardList : public ForwardContainer
{
public:
    ForwardList(Debugger* debugger, uint64_t address);
    void ClearBrowsingData() override;
    void Init(const std::string& containerVarExpr);
    int64_t Count(const std::string& containerVarExpr) override { return -1; }
    Iterator* Begin(const std::string& containerVarExpr) override;
    Iterator* End(const std::string& containerVarExpr) override;
private:
    std::unique_ptr<Pointer> headPtr;
    std::unique_ptr<Pointer> nullPtr;
};

class LinkedList : public ListContainer
{
public:
    LinkedList(Debugger* debugger, uint64_t address);
    void ClearBrowsingData() override;
    void Init(const std::string& containerVarExpr);
    Iterator* Begin(const std::string& containerVarExpr) override;
    Iterator* End(const std::string& containerVarExpr) override;
private:
    std::unique_ptr<Pointer> headPtr;
    std::unique_ptr<Pointer> nullPtr;
};

class HashMap : public HashtableContainer
{
public:
    HashMap(Debugger* debugger, uint64_t address);
};

class HashSet : public HashtableContainer
{
public:
    HashSet(Debugger* debugger, uint64_t address);
};

class ListCnt : public ListContainer
{
public:
    ListCnt(Debugger* debugger, uint64_t address);
    void ClearBrowsingData() override;
    Iterator* Begin(const std::string& containerVarExpr) override;
    Iterator* End(const std::string& containerVarExpr) override;
    std::unique_ptr<soul::xml::Element> Subscript(const std::string& containerVarExpr, int64_t index) override;
    std::string SubscriptExpressionString(const std::string& containerVarExpr, int64_t index) override;
private:
    std::unique_ptr<Pointer> itemsPtr;
};

class ListRepContainer : public Container
{
public:
    ListRepContainer(Debugger* debugger, cmajor::debug::ContainerClassTemplateKind kind, uint64_t address);
    int64_t Count(const std::string& containerVarExpr) override;
    Iterator* Begin(const std::string& containerVarExpr) override;
    Iterator* End(const std::string& containerVarExpr) override;
    std::unique_ptr<soul::xml::Element> Subscript(const std::string& containerVarExpr, int64_t index) override;
    std::string SubscriptExpressionString(const std::string& containerVarExpr, int64_t index) override;
private:
    ListCnt rep;
};

class Map : public TreeContainer
{
public:
    Map(Debugger* debugger, uint64_t address);
};

class Queue : public ListRepContainer
{
public:
    Queue(Debugger* debugger, uint64_t address);
};

class Set : public TreeContainer
{
public:
    Set(Debugger* debugger, uint64_t address);
};

class Stack : public ListRepContainer
{
public:
    Stack(Debugger* debugger, uint64_t address);
};

} // namespace cmajor::debug
