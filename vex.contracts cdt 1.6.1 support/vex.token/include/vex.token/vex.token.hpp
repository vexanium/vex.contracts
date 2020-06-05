#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace vexsystem {
   class system_contract;
}

namespace vex {

   using std::string;

   class [[eosio::contract("vex.token")]] token : public eosio::contract {
      public:
         using contract::contract;

  
         [[eosio::action]]
         void create( const eosio::name&   issuer,
                      const eosio::asset&  maximum_supply);
        
         [[eosio::action]]
         void issue( const eosio::name& to, const eosio::asset& quantity, const string& memo );

         
         [[eosio::action]]
         void retire( const eosio::asset& quantity, const string& memo );

         
         [[eosio::action]]
         void transfer( const eosio::name&    from,
                        const eosio::name&    to,
                        const eosio::asset&   quantity,
                        const string&  memo );
         
         [[eosio::action]]
         void open( const eosio::name& owner, const eosio::symbol& symbol, const eosio::name& ram_payer );

        
         [[eosio::action]]
         void close( const eosio::name& owner, const eosio::symbol& symbol );

         static eosio::asset get_supply( const eosio::name& token_contract_account, const eosio::symbol_code& sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         static eosio::asset get_balance( const eosio::name& token_contract_account, const eosio::name& owner, const eosio::symbol_code& sym_code )
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

         void sub_balance( const eosio::name& owner, const eosio::asset& value );
         void add_balance( const eosio::name& owner, const eosio::asset& value, const eosio::name& ram_payer );
   };

}
/// namespace vex
