#ifndef _GRID_UPDATE_MASK_H_
#define _GRID_UPDATE_MASK_H_

struct GridUpdateMask
{
	GridUpdateMask():create_int_mask_(0),create_string_mask_(0),update_int_mask_(0),update_string_mask_(0)
	{
		create_int_mask_ = new core_obj::UpdateMask;
		create_string_mask_ = new core_obj::UpdateMask;
		update_int_mask_ = new core_obj::UpdateMask;
		update_string_mask_ = new core_obj::UpdateMask;
		client_subscription_mask_ = new core_obj::UpdateMask;

		//整型下标掩码
		for (int i = 0; i < UNIT_FIELD_BUFF_TM; i++) {
			create_int_mask_->SetBit(i);
		}

		for (int i = UNIT_FIELD_HEALTH; i <= UNIT_FIELD_MAX_HEALTH; i++) {
			create_int_mask_->SetBit(i);
		}

		create_int_mask_->SetBit(UNIT_FIELD_ATTACK_SPEED);
		create_int_mask_->SetBit(UNIT_FIELD_MOVE_SPEED);
		create_int_mask_->SetBit(UINT_FIELD_VIRTUAL_CAMP);
		create_int_mask_->SetBit(UNIT_FIELD_FORCE);
		create_int_mask_->SetBit(UNIT_FIELD_FORCE+1);
		create_int_mask_->SetBit(UINT_INT_FIELD_ILLUSION);
		create_int_mask_->SetBit(UINT_INT_FIELD_WINGS_RANK);
		create_int_mask_->SetBit(UNIT_INT_FIELD_APPEARANCE);

		for (int i = 0; i < MAX_UNIT_STRING_FIELD; i++)
		{
			if(UNIT_STRING_FIELD_ALIAS_NAME == i  || GO_STRING_FIELD_CREATED_BY == i)
				continue;
			create_string_mask_->SetBit(i);
		}

		//更新包可以在创建包的基础上扣掉血量等等
		*update_int_mask_|= *create_int_mask_;	
		update_int_mask_->UnSetBit(UNIT_FIELD_HEALTH);
		update_int_mask_->SetBit(UNIT_FIELD_ANGER);
		update_int_mask_->SetBit(UINT_FIELD_XIANFU_INFO);
		for (int i = UNIT_FIELD_BUFF_RESERVE;i < UNIT_FIELD_FORCE; i++) {
			update_int_mask_->SetBit(i);
		}

		//字符串
		*update_string_mask_ |= *create_string_mask_;		
		
		//客户端查询专用返回掩码
		//血量、体力、法力、所有buff、精灵当前经验值、精灵最大经验值
		client_subscription_mask_->SetBit(UNIT_FIELD_HEALTH);
		//client_subscription_mask_->SetBit(UNIT_FIELD_POWERS_ENERGY);		
		//client_subscription_mask_->SetBit(UNIT_FIELD_MAXPOWERS_ENERGY);
		for (int i = UNIT_FIELD_BUFF_TM;i < UNIT_FIELD_FORCE; i++)
			client_subscription_mask_->SetBit(i);
	}

	~GridUpdateMask()
	{
		safe_delete(create_int_mask_);
		safe_delete(create_string_mask_);
		safe_delete(update_int_mask_);
		safe_delete(update_string_mask_);
		safe_delete(client_subscription_mask_);
	}

	core_obj::UpdateMask *create_int_mask_;
	core_obj::UpdateMask *create_string_mask_;

	core_obj::UpdateMask *update_int_mask_;
	core_obj::UpdateMask *update_string_mask_;

	core_obj::UpdateMask *client_subscription_mask_;
};

static GridUpdateMask gGridUpdateMask;

#endif
