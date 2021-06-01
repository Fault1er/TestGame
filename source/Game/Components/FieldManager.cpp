#include "FieldManager.hpp"


#include <Scene/Scene.hpp>
#include <Scene/Components/ClickComponent.hpp>
#include <Scene/Components/ButtonComponent.hpp>


namespace Game
{
	[[maybe_unused]]
	constexpr size_t BOARD_SIZE = 8u;


	bool FieldManager::CheckPos( const sf::Vector2i& pos )
	{
		return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
	}

	void FieldManager::ClickOnCell( int x, int y )
	{
		if ( player_color_ != pawn_color_move_ ) {
			return;
		}
		if ( cell_selected_ ) {
			MakeMove( player_color_, selected_cell_pos_, { x, y } );
			cell_selected_ = false;
		}
		else {
			auto* cell = cells_[y * BOARD_SIZE + x];
			if ( cell != nullptr && cell->HasPawn( pawn_color_move_ ) ) {
				cell_selected_ = true;
				selected_cell_pos_ = { x, y };
			}
		}
	}

	void FieldManager::SwitchMoveChoice()
	{
		pawn_color_move_ = pawn_color_move_ == Pawn::WHITE ? Pawn::BLACK : Pawn::WHITE;
	}

	void FieldManager::StartGame( Pawn player_color )
	{
		auto* scene = Scene::GetCurrentScene();
		if ( scene == nullptr ) {
			assert( false );
			return;
		}

		auto* field = scene->GetObject( "field" );
		if ( field == nullptr ) {
			assert( false );
			return;
		}

		cells_.reserve( BOARD_SIZE * BOARD_SIZE );
		auto& rows = field->GetChildren();
		assert( rows.size() == BOARD_SIZE );

		player_pawn_pos_.clear();
		player_target_pos_.clear();

		int y = 0;
		for ( auto& row_obj : rows )
		{
			int x = 0;
			auto& row = row_obj->GetChildren();
			assert( row.size() == BOARD_SIZE );
			for ( auto& cell_obj : row )
			{
				if ( auto* click = cell_obj->GetComponent<ClickComponent>() ) {
					click->SetAction( [this, x, y] () {
						ClickOnCell( x, y );
					} );
				}
				if ( auto* cell = cell_obj->GetComponent<Game::CellComponent>() )
				{
					cells_.push_back( cell );
					cell->SetPos( { x, y } );

					if ( !cell->HasPawn( Pawn::NONE ) )
					{
						if ( cell->HasPawn( player_color ) ) {
							player_pawn_pos_.emplace_back( x, y );
						}
						else {
							player_target_pos_.emplace_back( x, y );
						}
					}
					++x;
				}
			}
			++y;
		}

		ai_targets_.reserve( player_pawn_pos_.size() );
		for ( size_t i = 0u; i < player_pawn_pos_.size(); ++i ) {
			ai_targets_.emplace_back( player_target_pos_[i], player_pawn_pos_[i] );
		}

		pawn_color_move_ = Pawn::WHITE;
		player_color_ = player_color;
		cell_selected_ = false;
	}

	bool FieldManager::CheckFinish() const
	{
		if ( pawn_color_move_ == player_color_ ) {
			for ( const auto& target : player_target_pos_ ) {
				bool match = false;
				for ( const auto& pawn : player_pawn_pos_ ) {
					if ( target == pawn ) {
						match = true;
						break;
					}
				}
				if ( !match ) {
					return false;
				}
			}
		}
		else {
			for ( const auto& target: ai_targets_ ) {
				if ( target.pawn_pos != target.target_pos ) {
					return false;
				}
			}
		}

		return true;
	}

	void FieldManager::UpdateAI()
	{
		/// TODO: A-Star or equal
		int index = 0;
		int w_dist = -1;
		for ( auto& target : ai_targets_ ) {
			sf::Vector2i dir = target.target_pos - target.pawn_pos;
			bool have_dist = dir.x != 0 || dir.y != 0;
			if ( have_dist ) {
				sf::Vector2i x_axis = { target.pawn_pos.x + ( dir.x < 0 ? -1 : 1 ), target.pawn_pos.y };
				sf::Vector2i y_axis = { target.pawn_pos.x, target.pawn_pos.y + ( dir.y < 0 ? -1 : 1 ) };
				if ( std::abs( dir.x ) >= std::abs( dir.y ) && MakeMove( pawn_color_move_, target.pawn_pos, x_axis ) ) {
					target.pawn_pos = x_axis;
					return;
				}
				else if ( MakeMove( pawn_color_move_, target.pawn_pos, y_axis ) ) {
					target.pawn_pos = y_axis;
					return;
				}
				w_dist = index;
			}
			++index;
		}
		auto& target_wd = ai_targets_[w_dist];
		for ( auto& target : ai_targets_ ) {
			sf::Vector2i dir = target_wd.target_pos - target.pawn_pos;
			bool have_dist = dir.x != 0 || dir.y != 0;
			if ( have_dist ) {
				std::swap( target_wd.target_pos, target.target_pos );
				return;
			}
		}
	}


	FieldManager::FieldManager( GameObject* owner, const rapidjson::Document::GenericValue& node )
	: ObjectComponent( owner, node )
	{}


	void FieldManager::OnCreate()
	{
		auto* scene = Scene::GetCurrentScene();
		if ( scene == nullptr ) {
			assert( false );
			return;
		}

		auto* ui = scene->GetObject( "ui" );
		if ( ui == nullptr ) {
			assert( false );
			return;
		}

		auto* black_btn = ui->FindChild( "black_btn" );
		auto* white_btn = ui->FindChild( "white_btn" );

		if ( black_btn != nullptr && white_btn != nullptr )
		{
			black_btn->SetActive( true );
			white_btn->SetActive( true );

			auto* b_btn = black_btn->GetComponent<ButtonComponent>();
			auto* w_btn = white_btn->GetComponent<ButtonComponent>();

			if ( b_btn != nullptr && w_btn != nullptr )
			{
				b_btn->SetAction( [this, w_btn, b_btn, white_btn, black_btn] () {
					StartGame( Pawn::BLACK );
					w_btn->SetAction( nullptr );
					white_btn->SetActive( false );
					b_btn->SetAction( nullptr );
					black_btn->SetActive( false );
				} );
				w_btn->SetAction( [this, w_btn, b_btn, white_btn, black_btn] () {
					StartGame( Pawn::WHITE );
					w_btn->SetAction( nullptr );
					white_btn->SetActive( false );
					b_btn->SetAction( nullptr );
					black_btn->SetActive( false );
				} );
			}
		}

		pawn_color_move_ = Pawn::WHITE;
		player_color_ = Pawn::NONE;
		cell_selected_ = false;
	}

	void FieldManager::Update( float dt )
	{
		ObjectComponent::Update( dt );

		if ( pawn_color_move_ != player_color_ && player_color_ != Pawn::NONE ) {
			UpdateAI();
		}
	}

	void FieldManager::FinishGame()
	{
		for ( auto& cell : cells_ ) {
			auto* click = cell != nullptr ? cell->GetOwner()->GetComponent<ClickComponent>() : nullptr;
			if ( click != nullptr ) {
				click->SetAction( nullptr );
			}
		}
	}

	bool FieldManager::CanMove( const sf::Vector2i& pos, const sf::Vector2i& new_pos ) const
	{
		sf::Vector2i diff = new_pos - pos;

		if ( CheckPos( pos ) && CheckPos( new_pos ) && std::abs( diff.x + diff.y ) == 1 ) {
			auto* cell = cells_[new_pos.y * BOARD_SIZE + new_pos.x];
			return cell->HasPawn( Pawn::NONE );
		}

		return false;
	}

	bool FieldManager::MakeMove( Pawn pawn, const sf::Vector2i& pos, const sf::Vector2i& new_pos )
	{
		assert( pawn != Pawn::NONE );

		auto* cell = cells_[pos.y * BOARD_SIZE + pos.x];
		bool result = pawn == pawn_color_move_ && cell->HasPawn( pawn ) && CanMove( pos, new_pos );

		if ( result ) {
			auto* new_cell = cells_[new_pos.y * BOARD_SIZE + new_pos.x];
			cell->MoveToCell( new_cell );
			if ( CheckFinish() ) {
				FinishGame();
			}
			SwitchMoveChoice();
		}

		return result;
	}
}
