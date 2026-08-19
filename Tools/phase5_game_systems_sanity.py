#!/usr/bin/env python3
from dataclasses import dataclass, field
import random

@dataclass
class Part:
    id: str
    slot: str
    price: int
    torque: float = 1.0
    mass_delta: float = 0.0
    grip: float = 1.0
    requires: list[str] = field(default_factory=list)
    conflicts: list[str] = field(default_factory=list)

@dataclass
class Vehicle:
    id: str
    definition: str
    installed: dict[str, Part] = field(default_factory=dict)

@dataclass
class Profile:
    cash: int = 15000
    street_cred: int = 0
    garage: dict[str, Vehicle] = field(default_factory=dict)
    inventory: dict[str, Part] = field(default_factory=dict)
    ledger: list[tuple[str,int]] = field(default_factory=list)

    def tx(self, reason, amount):
        if amount < 0 and self.cash < -amount:
            return False
        self.cash += amount
        self.ledger.append((reason, amount))
        return True

    def buy_vehicle(self, listing_id, definition, price):
        if not self.tx(f'buy {listing_id}', -price): return None
        instance = f'veh_{len(self.garage)+1}'
        self.garage[instance] = Vehicle(instance, definition)
        return instance

    def buy_part(self, part: Part):
        if not self.tx(f'buy {part.id}', -part.price): return False
        self.inventory[part.id] = part
        return True

    def install(self, vehicle_id, part: Part):
        v = self.garage[vehicle_id]
        installed_ids = {p.id for p in v.installed.values()}
        if any(req not in installed_ids for req in part.requires): return False
        if any(conf in installed_ids for conf in part.conflicts): return False
        v.installed[part.slot] = part
        return True


def transfer_pinks(loser: Profile, winner: Profile, vehicle_id: str):
    if vehicle_id not in loser.garage or vehicle_id in winner.garage:
        return False
    bundle = loser.garage[vehicle_id]
    # Validate receiver first; mutation only happens once success is guaranteed.
    winner.garage[vehicle_id] = bundle
    del loser.garage[vehicle_id]
    return True


def opponent_plan(seed, reaction_mean=.22, reaction_var=.05, consistency=.85, nitrous_aggression=.5, pinks_accept=.1, risk=.5):
    r = random.Random(seed)
    reaction = max(-.2, min(.75, reaction_mean + r.uniform(-reaction_var, reaction_var)))
    shift_error = r.uniform(0,.12) * (1-max(0,min(1,consistency)))
    nitrous_delay = (1.25 + (0.05-1.25)*max(0,min(1,nitrous_aggression))) + r.uniform(0,.20)
    accept = r.random() < max(0,min(1,pinks_accept*(.5+risk)))
    return reaction, shift_error, nitrous_delay, accept


def validate_tune(launch, shift, tire, final_drive, boost, nitrous_delay, wheelie_bar, redline=7000):
    warnings=[]
    if launch < 800 or launch >= redline: warnings.append('launch')
    if shift <= launch or shift > redline*1.05: warnings.append('shift')
    if not 6 <= tire <= 45: warnings.append('tire')
    if not 2 <= final_drive <= 6.5: warnings.append('final')
    if not 0 <= boost <= 80: warnings.append('boost')
    if not 0 <= nitrous_delay <= 10: warnings.append('nitrous')
    if not 0 <= wheelie_bar <= .30: warnings.append('wheelie')
    return warnings


def resolve_build(base_mass, parts):
    torque = grip = 1.0
    mass = base_mass
    for p in parts:
        torque *= p.torque; grip *= p.grip; mass += p.mass_delta
    return mass, torque, grip


def main():
    p = Profile()
    car = p.buy_vehicle('starter_listing','starter_rwd',5000)
    assert car and p.cash == 10000

    intake = Part('intake_stage1','intake',500,torque=1.04)
    turbo = Part('turbo_street','forced_induction',3500,torque=1.28,requires=['intake_stage1'])
    blower = Part('blower_street','forced_induction',3800,torque=1.25,conflicts=['turbo_street'])
    slick = Part('slick_275','tire',900,grip=1.18)
    weight = Part('rear_seat_delete','weight',250,mass_delta=-28)

    assert not p.install(car,turbo)
    assert p.buy_part(intake) and p.install(car,intake)
    assert p.buy_part(turbo) and p.install(car,turbo)
    assert not p.install(car,blower)
    assert p.buy_part(slick) and p.install(car,slick)
    assert p.buy_part(weight) and p.install(car,weight)

    mass, torque, grip = resolve_build(1600,[intake,turbo,slick,weight])
    assert mass == 1572 and torque > 1.30 and grip > 1.10

    before = p.cash
    assert not p.tx('overspend',-(before+1)) and p.cash == before
    assert p.tx('race entry',-500) and p.tx('race win',2200)
    p.street_cred += 125
    tiers = [(1,0),(2,100),(3,300),(4,700)]
    level = max(level for level, cred in tiers if p.street_cred >= cred)
    assert level == 2

    # Pink transfer must move one and only one ownership record.
    rival = Profile(cash=5000)
    assert transfer_pinks(p, rival, car)
    assert car not in p.garage and car in rival.garage
    assert not transfer_pinks(p, rival, car)

    # Same seed means same AI plan, allowing exact race reproduction.
    a = opponent_plan(1320)
    b = opponent_plan(1320)
    c = opponent_plan(1321)
    assert a == b and a != c

    assert validate_tune(3200,6800,18,3.55,18,.4,.08) == []
    assert len(validate_tune(750,700,55,8,100,20,.5)) >= 5

    print('Phase 5 game systems sanity: PASS')
    print(f'cash={p.cash} street_cred={p.street_cred} level={level}')
    print(f'mass={mass:.0f}kg torque_mult={torque:.3f} grip_mult={grip:.3f}')
    print(f'opponent_plan={a[:3]} pink_transfer_owner={"rival" if car in rival.garage else "invalid"}')
    print(f'ledger_entries={len(p.ledger)}')

if __name__ == '__main__':
    main()
