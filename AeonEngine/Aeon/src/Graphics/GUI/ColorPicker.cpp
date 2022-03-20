#include <AEON/Graphics/GUI/ColorPicker.h>

#include <AEON/Window/Application.h>

namespace ae
{
	// Public constructor(s)
	ColorPicker::ColorPicker(const Vector2f& size, size_t sliceCount)
		: Actor()
		, mSlices(sliceCount)
		, mSliderBackgroundSlices(sliceCount)
		, mHueSlider(nullptr)
		, mSelection(nullptr)
		, mHue(0.f)
		, mSelectionHeld(false)
	{
		init(size);
	}

	// Public method(s)
	void ColorPicker::setSize(const Vector2f& size)
	{
		Collider2DComponent* const collider = getComponent<Collider2DComponent>();
		if (collider->getModelBounds().size != size) {
			Transform2DComponent* const selectionTransform = mSelection->getComponent<Transform2DComponent>();
			selectionTransform->setPosition(selectionTransform->getPosition().xy / collider->getModelBounds().size * size);

			collider->setModelBounds(Box2f(0.f, 0.f, size.x, size.y));
			updateSlices();
			updateSliderBackgroundSlices();

			mHueSlider->setLimits(0, 360, size.x);
		}
	}

	void ColorPicker::setSliceCount(size_t count)
	{
		if (mSlices.size() != count) {
			mSlices.resize(count);
			mSliderBackgroundSlices.resize(count);
			updateSlices();
			updateSliderBackgroundSlices();

			attachChild(std::move(detachChild(*mSelection)));
			if (!mSliderBackgroundSlices.empty() && mSliderBackgroundSlices.front()) {
				attachChild(std::move(detachChild(*mSliderBackgroundSlices.front())));
			}
		}
	}

	void ColorPicker::setHue(float hue)
	{
		mHue = hue;
		updateSlices();
		updateSelection();
		mHueSlider->setValue(static_cast<int>(hue));

		for (uint32_t i = 0; i < Button<EllipseShape>::State::StateCount; ++i) {
			EllipseShape& state = mHueSlider->getState(static_cast<Button<EllipseShape>::State>(i));
			state.setFillColor(Color::fromHSV(hue, 100.f, 100.f));
		}
	}

	void ColorPicker::setColor(const Color& color)
	{
		if (mSelection->getFillColor() != color) {
			// Convert RGB color to HSV
			float hue, saturation, value;
			uint8_t alpha;
			color.toHSV(hue, saturation, value, alpha);

			// Update selection position and slices
			const Vector2f& size = getComponent<Collider2DComponent>()->getModelBounds().size;
			mSelection->getComponent<Transform2DComponent>()->setPosition(saturation * size.x / 100.f, (100.f - value) * size.y / 100.f);
			setHue(hue);
		}
	}

	// Private method(s)
	void ColorPicker::updateSlices()
	{
		const Vector2f& size = getComponent<Collider2DComponent>()->getModelBounds().size;
		const Vector2f SLICE_SIZE(size.x / mSlices.size(), size.y);

		for (size_t i = 0; i < mSlices.size(); ++i) {
			if (mSlices[i]) {
				mSlices[i]->setSize(SLICE_SIZE);
			}
			else {
				auto slice = std::make_unique<RectangleShape>(SLICE_SIZE);
				mSlices[i] = slice.get();
				attachChild(std::move(slice));
			}
			mSlices[i]->getComponent<Transform2DComponent>()->setPosition(SLICE_SIZE.x * i, 0.f);

			// Update color
			std::vector<Vertex2D>& vertices = mSlices[i]->getComponent<Render2DComponent>()->getVertices();
			const float Smin = i * 100.f / mSlices.size();
			const float Smax = (i + 1) * 100.f / mSlices.size();

			vertices[0].color = Color::fromHSV(mHue, Math::lerp(Smin, Smax, 0.5f), 100.f / 2.f).normalize();
			vertices[1].color = Color::fromHSV(mHue, Smax, 100.f).normalize();
			vertices[2].color = Color::fromHSV(mHue, Smin, 100.f).normalize();
			vertices[3].color = Color::fromHSV(mHue, Smin, 0.f).normalize();
			vertices[4].color = Color::fromHSV(mHue, Smax, 0.f).normalize();

			vertices[0].position = Vector2f(SLICE_SIZE.x / mSlices.size() / 2.f, SLICE_SIZE.y / 2.f);
		}
	}

	void ColorPicker::updateSliderBackgroundSlices()
	{
		const Vector2f& size = getComponent<Collider2DComponent>()->getModelBounds().size;
		const Vector2f SLICE_SIZE(size.x / mSlices.size(), mSelection->getRadius().y / 2.f);

		for (int64_t i = mSliderBackgroundSlices.size() - 1; i >= 0; --i) {
			if (mSliderBackgroundSlices[i]) {
				mSliderBackgroundSlices[i]->setSize(SLICE_SIZE);
			}
			else {
				auto slice = std::make_unique<RectangleShape>(SLICE_SIZE);
				mSliderBackgroundSlices[i] = slice.get();
				attachChild(std::move(slice));
			}
			mSliderBackgroundSlices[i]->getComponent<Transform2DComponent>()->setPosition(SLICE_SIZE.x * i, size.y + 15.f);

			// Update color
			std::vector<Vertex2D>& vertices = mSliderBackgroundSlices[i]->getComponent<Render2DComponent>()->getVertices();
			const float Hmin = i * 360.f / mSliderBackgroundSlices.size();
			const float Hmax = (i + 1) * 360.f / mSliderBackgroundSlices.size();

			vertices[0].color = Color::fromHSV(Math::lerp(Hmin, Hmax, 0.5f), 100.f, 100.f).normalize();
			vertices[1].color = Color::fromHSV(Hmax, 100.f, 100.f).normalize();
			vertices[2].color = Color::fromHSV(Hmin, 100.f, 100.f).normalize();
			vertices[3].color = Color::fromHSV(Hmin, 100.f, 100.f).normalize();
			vertices[4].color = Color::fromHSV(Hmax, 100.f, 100.f).normalize();

			vertices[0].position = Vector2f(SLICE_SIZE.x / mSlices.size() / 2.f, SLICE_SIZE.y / 2.f);
		}
	}

	void ColorPicker::updateSelection()
	{
		const Vector2f& size = getComponent<Collider2DComponent>()->getModelBounds().size;
		const Vector3f& pos = mSelection->getComponent<Transform2DComponent>()->getPosition();

		mSelection->setFillColor(Color::fromHSV(mHue, pos.x * 100.f / size.x, 100.f - pos.y * 100.f / size.y));
	}

	void ColorPicker::init(const Vector2f& size)
	{
		addComponent<Transform2DComponent>();
		addComponent<Collider2DComponent>();

		// Create the slices
		getComponent<Collider2DComponent>()->setModelBounds(Box2f(0.f, 0.f, size.x, size.y));
		updateSlices();

		// Create selection
		auto selection = std::make_unique<EllipseShape>(Vector2f(8.f, 8.f), 30);
		selection->setOutlineColor(Color::White);
		selection->setOutlineThickness(1.75f);
		mSelection = selection.get();
		attachChild(std::move(selection));

		Transform2DComponent* const selectionTransform = mSelection->getComponent<Transform2DComponent>();
		selectionTransform->setOriginFlags(Transform2DComponent::OriginType::All, Transform2DComponent::OriginFlag::Center);

		updateSelection();

		// Create the hue slider
		updateSliderBackgroundSlices();
		auto hueSlider = std::make_unique<IntSlider>(0, 360, 0, size.x);
		for (uint32_t i = 0; i < Button<EllipseShape>::State::StateCount; ++i) {
			EllipseShape& state = hueSlider->getState(static_cast<Button<EllipseShape>::State>(i));
			state.setRadius(mSelection->getRadius());
			state.setPointCount(30);
			state.setOutlineColor(Color::White);
			state.setOutlineThickness(1.75f);
		}
		mHueSlider = hueSlider.get();
		mSliderBackgroundSlices.front()->attachChild(std::move(hueSlider));

		Transform2DComponent* const hueSliderTransform = mHueSlider->getComponent<Transform2DComponent>();
		hueSliderTransform->setOriginFlags(Transform2DComponent::OriginType::All, Transform2DComponent::OriginFlag::Center);

		mHueSlider->enable(false);
		mHueSlider->enable(true);
	}

	// Private virtual method(s)
	void ColorPicker::updateSelf(const Time& dt)
	{
		const float HUE_SLIDER_VALUE = static_cast<float>(mHueSlider->getValue());
		if (mHue != HUE_SLIDER_VALUE) {
			mHue = HUE_SLIDER_VALUE;
			updateSlices();
			updateSelection();

			for (uint32_t i = 0; i < Button<EllipseShape>::State::StateCount; ++i) {
				EllipseShape& state = mHueSlider->getState(static_cast<Button<EllipseShape>::State>(i));
				state.setFillColor(Color::fromHSV(HUE_SLIDER_VALUE, 100.f, 100.f));
			}
		}

		Actor::updateSelf(dt);
	}

	void ColorPicker::handleEventSelf(Event* const event)
	{
		if (!event->handled && event->type == Event::Type::MouseButtonPressed) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				const Box2f GLOBAL_BOUNDS = getComponent<Collider2DComponent>()->getGlobalBounds();
				const Vector2f MOUSE_COORDS = Application::getInstance().getWindow().mapPixelToCoords(Mouse::getPosition());
				if (GLOBAL_BOUNDS.contains(MOUSE_COORDS)) {
					mSelection->getComponent<Transform2DComponent>()->setPosition(MOUSE_COORDS - GLOBAL_BOUNDS.position);
					updateSelection();

					mSelectionHeld = true;
					event->handled = true;
				}
			}
		}
		else if (mSelectionHeld) {
			if (event->type == Event::Type::MouseMoved) {
				const Vector2f PICKER_POS = getComponent<Collider2DComponent>()->getGlobalBounds().position;
				const Vector2f MOUSE_POS = event->as<MouseMoveEvent>()->position;
				const Vector2f& pickerSize = getComponent<Collider2DComponent>()->getModelBounds().size;

				const Vector2f SELECTION_POS(Math::clamp(MOUSE_POS.x - PICKER_POS.x, 0.f, pickerSize.x),
				                             Math::clamp(MOUSE_POS.y - PICKER_POS.y, 0.f, pickerSize.y));
				mSelection->getComponent<Transform2DComponent>()->setPosition(SELECTION_POS);
				updateSelection();
			}
			else if (event->type == Event::Type::MouseButtonReleased) {
				mSelectionHeld = false;
			}
		}
	}
}