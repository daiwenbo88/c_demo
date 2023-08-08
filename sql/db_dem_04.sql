select * from ecs_goods;


#查询商品id的最大价格
select cat_id,max(shop_price) from ecs_goods group by cat_id;

#查询ecshop 中各个类别 价格最高的商品
select goods_id,ecs_goods.cat_id,goods_name,shop_price from (
        select cat_id,max(shop_price) as max_price from ecs_goods
                                      group by cat_id
        ) temp,ecs_goods
        where  temp.cat_id=ecs_goods.cat_id
        and    temp.max_price =ecs_goods.shop_price;