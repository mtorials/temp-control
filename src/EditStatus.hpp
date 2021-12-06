#pragma once

#include "EditableValues.hpp"

class EditStatus {
private:
  EditableValues currentlyEditing;
public:
  EditStatus(EditableValues initalValue) {
    currentlyEditing = initalValue;
  }
  EditableValues getValue() {
    return currentlyEditing;
  }
  void setValue(EditableValues newValue) {
    currentlyEditing = newValue;
  }
};