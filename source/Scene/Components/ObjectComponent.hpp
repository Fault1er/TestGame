#pragma once


#include <Utils/rapidjson/document.h>


#include <memory>
#include <functional>


class GameObject;


class ObjectComponent
{
private:
	static void Deleter( ObjectComponent* );

protected:
	GameObject* owner_ = nullptr;

public:
	using Ptr = std::unique_ptr<ObjectComponent, std::function<void( ObjectComponent* component )>>;

public:
	explicit ObjectComponent( GameObject* owner, const rapidjson::Document::GenericValue& node ) : owner_( owner ) {};
	virtual ~ObjectComponent() = default;

	[[nodiscard]]
	auto GetOwner() const -> GameObject*;

	/// Вызывается после создания объекта
    virtual void OnCreate() {}

    /// Вызывается после OnCreate и при включении объекта
    virtual void OnEnable() {}

    /// Вызывается каждый кадр
    virtual void Update( float dt ) {}

    /// Вызывается перед OnDestroy при выключении объекта
    virtual void OnDisable() {}

    /// Вызывается перед удалением объекта
    virtual void OnDestroy() {}

    /// Фабрика по созданию компонентов
	static auto CreateComponent( GameObject* owner, const rapidjson::Document::GenericValue& node ) -> ObjectComponent::Ptr;
};

