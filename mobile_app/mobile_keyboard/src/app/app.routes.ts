import { Routes } from '@angular/router';

export const routes: Routes = [
  {
    path: '',
    redirectTo: 't1',
    pathMatch: 'full',
  },

  {
    path: 't1',
    loadComponent: () => import('./t1/t1.page').then( m => m.T1Page)
  },
  {
    path: 'new-pin',
    loadComponent: () => import('./new-pin/new-pin.page').then( m => m.NewPinPage)
  },
];
