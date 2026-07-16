export module entity;

#pragma once

export class Entity {
 public:
  virtual ~Entity() = 0;
};

Entity::~Entity() = default;
