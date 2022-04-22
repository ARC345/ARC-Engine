#pragma once

namespace ARC {
	class CScene
	{
	public:
		CScene();
		~CScene();

	private:
		//using MyComponents = ECS::IMPL::ClassList<HPR::CustomCounter<ECS::ComponentCounterId>::Next() - 1>::type;
		//using MySignatures = ECS::IMPL::ClassList<HPR::CustomCounter<ECS::SignatureCounterId>::Next() - 1>::type;
		//using MyTags = ECS::IMPL::ClassList<HPR::CustomCounter<ECS::TagCounterId>::Next() - 1>::type;
		//using MyManager = ECS::CManager<ECS::SSettings<MyComponents, MyTags, MySignatures>>
	};
}