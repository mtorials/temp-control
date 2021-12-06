#pragma once

enum EditableValues
{
  Tmp0,
  Tmp1,
  Tmp2,
  Rmp1,
  Rmp2,
  Rmp3,
  T1,
  T2,
  T3
};

class EditStatus
{
private:
  EditableValues currentlyEditing;

public:
  EditStatus(EditableValues initalValue)
  {
    currentlyEditing = initalValue;
  }
  EditableValues getValue()
  {
    return currentlyEditing;
  }
  void setValue(EditableValues newValue)
  {
    currentlyEditing = newValue;
  }
};