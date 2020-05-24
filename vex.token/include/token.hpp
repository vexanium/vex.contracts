#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>

namespace vexsystem {
   class system_contract;
}

namespace vex {

   using std::string;

   class [[eosio::contract("token")]] token : public eosio::contract {
      public:
         using contract::contract;

         [[eosio::action]]
         void create( eosio::name   issuer,
                      eosio::asset  maximum_supply);

         [[eosio::action]]
         void issue( eosio::name to, eosio::asset quantity, string memo );

         [[eosio::action]]
         void retire( eosio::asset quantity, string memo );

         [[eosio::action]]
         void transfer( eosio::name    from,
                        eosio::name    to,
                        eosio::asset   quantity,
                        string  memo );

         [[eosio::action]]
         void open( eosio::name owner, const eosio::symbol& symbol, eosio::name ram_payer );

         [[eosio::action]]
         void close( eosio::name owner, const eosio::symbol& symbol );

         static eosio::asset get_supply( eosio::name token_contract_account, eosio::symbol_code sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         static eosio::asset get_balance( eosio::name token_contract_account, eosio::name owner, eosio::symbol_code sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

         using create_action = eosio::action_wrapper<"create"_n, &token::create>;
         using issue_action = eosio::action_wrapper<"issue"_n, &token::issue>;
         using retire_action = eosio::action_wrapper<"retire"_n, &token::retire>;
         using transfer_action = eosio::action_wrapper<"transfer"_n, &token::transfer>;
         using open_action = eosio::action_wrapper<"open"_n, &token::open>;
         using close_action = eosio::action_wrapper<"close"_n, &token::close>;
      private:
         struct [[eosio::table]] account {
            eosio::asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table]] currency_stats {
            eosio::asset    supply;
            eosio::asset    max_supply;
            eosio::name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;

         void sub_balance( eosio::name owner, eosio::asset value );
         void add_balance( eosio::name owner, eosio::asset value, eosio::name ram_payer );
   };

} /// namespace vex
